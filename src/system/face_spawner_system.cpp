#include <random>
#include <algorithm>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "face_spawner_system.h"


namespace gamee {


FaceSpawnerSystem::FaceSpawnerSystem()
    : generator{std::random_device{}()},
      faces{FaceType::ANGRY, FaceType::DISGUSTED, FaceType::FEARFUL, FaceType::HAPPY, FaceType::RESTED, FaceType::SAD, FaceType::SURPRISED},
      next{length}
{}


void FaceSpawnerSystem::update(Registry &registry, delta_type delta) {
    auto view = registry.view<SpawnRequest>();

    view.each([&, this](auto, auto &request) {
        request.elapsed += delta;

        if(!(request.elapsed < request.interval)) {
            auto &textureCache = Locator::TextureCache::ref();

            if(next == length) {
                std::shuffle(faces, faces+length, generator);
                next = 0_ui8;
            }

            auto spawn = [&, this](auto handle, auto type) {
                const int dstX = request.target.x + (generator() % request.target.w) - (handle->width() / 2);
                const int dstY = request.target.y + (generator() % request.target.h) - (handle->height() / 2);

                int srcX, srcY;

                if(request.horizontal) {
                    if(request.rightBottom) {
                        // bottom
                        srcX = (generator() % request.width) + request.from - (handle->width() / 2);
                        srcY = logicalHeight;
                    } else {
                        // top
                        srcX = (generator() % request.width) + request.from - (handle->width() / 2);
                        srcY = -handle->height();
                    }
                } else {
                    if(request.rightBottom) {
                        // right
                        srcX = logicalWidth;
                        srcY = (generator() % request.width) + request.from - (handle->height() / 2);
                    } else {
                        // left
                        srcX = -handle->width();
                        srcY = (generator() % request.width) + request.from - (handle->height() / 2);
                    }
                }

                const float dX = dstX - srcX;
                const float dY = dstY - srcY;
                const float gravity = .001f;

                auto entity = registry.create<Renderable>();
                registry.assign<Transform>(entity, 1.f * srcX, 1.f * srcY);
                registry.assign<Movement>(entity, gravity, 1.f * dX / request.xt, 1.f * dY / request.yt);
                registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
                registry.assign<BoundingBox>(entity, handle->width(), handle->height());
                registry.assign<FaceSmash>(entity, type);
            };

            switch (faces[next]) {
            case FaceType::ANGRY:
                spawn(textureCache.handle("emoji/angry"), faces[next]);
                break;
            case FaceType::DISGUSTED:
                spawn(textureCache.handle("emoji/disgusted"), faces[next]);
                break;
            case FaceType::HAPPY:
                spawn(textureCache.handle("emoji/happy"), faces[next]);
                break;
            case FaceType::RESTED:
                spawn(textureCache.handle("emoji/rested"), faces[next]);
                break;
            case FaceType::SURPRISED:
                spawn(textureCache.handle("emoji/surprised"), faces[next]);
                break;
            case FaceType::FEARFUL:
                spawn(textureCache.handle("emoji/fearful"), faces[next]);
                break;
            case FaceType::SAD:
                spawn(textureCache.handle("emoji/sad"), faces[next]);
                break;
            }

            request.elapsed = 0_ui32;
            ++next;
        }
    });
}


}
