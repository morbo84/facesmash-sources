#include <random>
#include <algorithm>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "face_spawner_system.h"


namespace gamee {


void FaceSpawnerSystem::spawn(Registry &registry, const SpawnRequest &request, TextureCache::resource_type resource, FaceType type, FaceModifier modifier) {
    auto handle = Locator::TextureCache::ref().handle(resource);

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
    registry.assign<FaceSmash>(entity, type, modifier);
}



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
            if(next == length) {
                std::shuffle(faces, faces+length, generator);
                next = 0_ui8;
            }

            // unfortunately we haven't special versions for each and every face
            // therefore modifiers come before instead of along with the face

            switch(request.modifier) {
            case FaceModifier::SPEED_UP:
                spawn(registry, request, "emoji/pouting", FaceType::ANGRY, request.modifier);
                break;
            case FaceModifier::SLOW_DOWN:
                spawn(registry, request, "emoji/star", FaceType::HAPPY, request.modifier);
                break;
            case FaceModifier::NONE:
                switch (faces[next]) {
                case FaceType::ANGRY:
                    spawn(registry, request, "emoji/angry", faces[next], FaceModifier::NONE);
                    break;
                case FaceType::DISGUSTED:
                    spawn(registry, request, "emoji/disgusted", faces[next], FaceModifier::NONE);
                    break;
                case FaceType::HAPPY:
                    spawn(registry, request, "emoji/happy", faces[next], FaceModifier::NONE);
                    break;
                case FaceType::RESTED:
                    spawn(registry, request, "emoji/rested", faces[next], FaceModifier::NONE);
                    break;
                case FaceType::SURPRISED:
                    spawn(registry, request, "emoji/surprised", faces[next], FaceModifier::NONE);
                    break;
                case FaceType::FEARFUL:
                    spawn(registry, request, "emoji/fearful", faces[next], FaceModifier::NONE);
                    break;
                case FaceType::SAD:
                    spawn(registry, request, "emoji/sad", faces[next], FaceModifier::NONE);
                    break;
                }
                break;
            };

            request.elapsed = 0_ui32;
            ++next;
        }
    });
}


}
