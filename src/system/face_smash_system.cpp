#include <SDL_rect.h>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../factory/common.h"
#include "../factory/play_factory.h"
#include "../factory/spawner.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "face_smash_system.h"


namespace gamee {


FaceSmashSystem::FaceSmashSystem()
    : type{FaceType::HAPPY},
      dirty{false}
{
    Locator::Dispatcher::ref().connect<FaceEvent>(this);
}


FaceSmashSystem::~FaceSmashSystem() {
    Locator::Dispatcher::ref().disconnect<FaceEvent>(this);
}


void FaceSmashSystem::receive(const FaceEvent &event) noexcept {
    type = event.type;
    dirty = true;
}


void FaceSmashSystem::update(Registry &registry, Spawner &spawner) {
    auto &textureCache = Locator::TextureCache::ref();

    auto view = registry.view<Face, Transform, BoundingBox>();
    SmashEvent event{};

    view.each([&, this](auto entity, const auto &smash, const auto &transform, const auto &box) {
        const auto area = transformToPosition(registry, entity, transform) * box;

        if(registry.has<Destroyable>(entity)) {
            const auto x = area.x + area.w / 2;
            const auto y = area.y + area.h / 2;

            if(SDL_HasIntersection(&playArea, &area)) {
                if(dirty && smash.type == type) {
                    spawner.spawnExplosion(registry, x, y);
                    spawner.spawnSmashScore(registry, smash.smash, x, y);

                    switch(smash.type) {
                    case FaceType::ANGRY:
                        ++event.angry;
                        break;
                    case FaceType::DISGUSTED:
                        ++event.disgusted;
                        break;
                    case FaceType::HAPPY:
                        ++event.happy;
                        break;
                    case FaceType::SURPRISED:
                        ++event.surprised;
                        break;
                    case FaceType::FEARFUL:
                        ++event.fearful;
                        break;
                    case FaceType::SAD:
                        ++event.sad;
                        break;
                    };

                    event.smash += smash.smash;
                    ++event.combo;

                    registry.destroy(entity);
                }
            } else if(!SDL_HasIntersection(&logicalScreen, &area)) {
                spawner.spawnMissScore(registry, smash.miss, x, y);
                event.miss += smash.miss;
                registry.destroy(entity);
            }
        } else if(!SDL_HasIntersection(&logicalScreen, &area)) {
            registry.destroy(entity);
        }
    });

    dirty = false;

    // check rewards
    auto reward = [&](auto handle, auto shake) {
        auto &camera = registry.get<Camera>();
        camera.remaining = shakeDuration;
        camera.shake = shake;

        auto entity = createInGameMessage(registry, handle, 160);
        const auto &sprite = registry.get<Sprite>(entity);
        setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight / 4 - sprite.h) / 2);
    };

    if(1 == event.combo && registry.empty<Face>()) {
        // no more faces to smash
        reward(textureCache.handle("str/reward/perfect"), 8_ui8);
    } else if(2 == event.combo) {
        // 2x combo
        reward(textureCache.handle("str/reward/x2"), 2_ui8);
    } else if(3 == event.combo) {
        // 3x combo
        reward(textureCache.handle("str/reward/x3"), 4_ui8);
    } else if(4 == event.combo) {
        // 4x combo
        reward(textureCache.handle("str/reward/x4"), 6_ui8);
    } else if(5 <= event.combo) {
        // 5x combo
        reward(textureCache.handle("str/reward/x5"), 8_ui8);
    }

    Locator::Dispatcher::ref().enqueue<SmashEvent>(event);
}


}
