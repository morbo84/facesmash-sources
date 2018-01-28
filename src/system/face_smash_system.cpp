#include <SDL_rect.h>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../factory/play_factory.h"
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


void FaceSmashSystem::update(Registry &registry, PlayFactory &factory) {
    auto &score = registry.get<PlayerScore>();
    auto &textureCache = Locator::TextureCache::ref();
    int total = 0;
    int combo = 0;

    auto view = registry.view<Face, Transform, BoundingBox>();

    view.each([&, this](auto entity, const auto &smash, const auto &transform, const auto &box) {
        const auto area = transformToPosition(registry, entity, transform) * box;

        if(registry.has<Destroyable>(entity)) {
            const auto x = area.x + area.w / 2;
            const auto y = area.y + area.h / 2;

            if(SDL_HasIntersection(&playArea, &area)) {
                if(dirty && smash.type == type) {
                    factory.spawnExplosion(registry, x, y);
                    factory.spawnSmashScore(registry, smash.smash, x, y);

                    switch(smash.type) {
                    case FaceType::ANGRY:
                        ++score.hitAngry;
                        break;
                    case FaceType::DISGUSTED:
                        ++score.hitDisgusted;
                        break;
                    case FaceType::HAPPY:
                        ++score.hitHappy;
                        break;
                    case FaceType::SURPRISED:
                        ++score.hitSurprised;
                        break;
                    case FaceType::FEARFUL:
                        ++score.hitFearful;
                        break;
                    case FaceType::SAD:
                        ++score.hitSad;
                        break;
                    };

                    ++combo;
                    total += smash.smash;
                    score.score += smash.smash;

                    registry.destroy(entity);
                }
            } else if(!SDL_HasIntersection(&logicalScreen, &area)) {
                factory.spawnMissScore(registry, smash.miss, x, y);
                score.score = (smash.miss > score.score) ? 0 : (score.score - smash.miss);
                registry.destroy(entity);
            }
        } else if(!SDL_HasIntersection(&logicalScreen, &area)) {
            registry.destroy(entity);
        }
    });

    dirty = false;

    // check rewards
    auto gotIt = [&](auto handle, auto amount) {
        registry.accomodate<Sprite>(registry.attachee<Reward>(), handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.get<Reward>().dirty = true;
        score.score += amount;
    };

    if(1 == combo && registry.empty<Face>()) {
        // no more faces to smash
        gotIt(textureCache.handle("str/reward/perfect"), total);
    } else if(2 == combo) {
        // 2x combo
        gotIt(textureCache.handle("str/reward/x2"), 2 * total);
    } else if(3 == combo) {
        // 3x combo
        gotIt(textureCache.handle("str/reward/x3"), 3 * total);
    } else if(4 == combo) {
        // 4x combo
        gotIt(textureCache.handle("str/reward/x4"), 4 * total);
    } else if(5 <= combo) {
        // 5x combo
        gotIt(textureCache.handle("str/reward/x5"), 5 * total);
    }
}


}
