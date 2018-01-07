#include <SDL_rect.h>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "face_smash_system.h"


namespace gamee {


void FaceSmashSystem::addScore(Registry &registry, SDLTextureHandle handle, SDL_Point pos) {
    auto entity = registry.create();

    registry.assign<Renderable>(entity);
    registry.assign<Movement>(entity, 0.f, 0.f, -.06f);
    registry.assign<FadeAnimation>(entity, 255, 0, 2000_ui32);
    registry.assign<DestroyLater>(entity, 2000_ui32);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());

    auto &transform = registry.assign<Transform>(entity, 1.f * pos.x - handle->width() / 2, 1.f * pos.y - handle->height() / 2);
    const auto left = transform.x;
    const auto right = transform.x + handle->width();
    const auto bottom = transform.y + handle->height();
    transform.x -= (left < 0.f) ? left : 0.f;
    transform.x -= (right > logicalScreen.w) ? (right - logicalScreen.w) : 0.f;
    transform.y -= (bottom > logicalScreen.h) ? (bottom - logicalScreen.h) : 0.f;
}


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


void FaceSmashSystem::update(Registry &registry) {
    if(registry.has<LetsPlay>()) {
        assert(registry.has<PlayerScore>());

        const auto &play = registry.get<LetsPlay>();
        auto &score = registry.get<PlayerScore>();
        auto &textureCache = Locator::TextureCache::ref();
        const SDL_Rect screen = logicalScreen;
        int total = 0;
        int combo = 0;

        auto view = registry.view<FaceSmash, Transform, Movement, BoundingBox>();

        view.each([&, this](auto entity, const auto &smash, const auto &transform, const auto &movement, const auto &box) {
            const auto area = transform * box;
            SDL_Point center = { area.x + area.w / 2, area.y + area.h / 2 };

            if(movement.velY > 0 && !SDL_HasIntersection(&screen, &area)) {
                switch(smash.miss) {
                case 50:
                    addScore(registry, textureCache.handle("miss/50"), center);
                    break;
                    // TODO other scores ...
                }

                score.score = (smash.miss > score.score) ? 0 : (score.score - smash.miss);
                registry.destroy(entity);
            } else if(// an emotion has been detected in the meantime
                      dirty &&
                      // the types match each with the other
                      smash.type == type &&
                      // the face is within the smash area
                      SDL_HasIntersection(&play.smashArea, &area))
            {
                auto explosion = registry.create();
                registry.assign<Sprite>(explosion, textureCache.handle("game/explosion"), 192, 192, 192, 192, 0, 0, 20_ui8, 5_ui8);
                registry.assign<Renderable>(explosion, 0.f, 255);
                registry.assign<Transform>(explosion, area.x + area.w / 2.f - 96, area.y + area.h / 2.f - 96);
                registry.assign<SpriteAnimation>(explosion, 1000_ui32, 0_ui32, false);
                registry.assign<DestroyLater>(explosion, 1000_ui32);

                switch(smash.smash) {
                case 100:
                    addScore(registry, textureCache.handle("smash/100"), center);
                    break;
                    // TODO other scores ...
                }

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

                switch(smash.modifier) {
                case FaceModifier::NONE:
                    break;
                case FaceModifier::SLOW_DOWN:
                    Locator::Dispatcher::ref().enqueue<FaceModifierEvent>(FaceModifier::SLOW_DOWN, 5000_ui32);
                    break;
                case FaceModifier::SPEED_UP:
                    Locator::Dispatcher::ref().enqueue<FaceModifierEvent>(FaceModifier::SPEED_UP, 1000_ui32);
                    break;
                };

                registry.destroy(entity);
            }
        });

        // check bonuses
        auto gotIt = [&](auto handle, auto amount) {
            registry.accomodate<Sprite>(registry.attachee<BonusSmash>(), handle, handle->width(), handle->height(), handle->width(), handle->height());
            registry.get<BonusSmash>().dirty = true;
            score.score += amount;
        };

        if(1 == combo && registry.empty<FaceSmash>()) {
            // no more faces to smash
            gotIt(textureCache.handle("bonus/perfect"), total);
        } else if(2 == combo) {
            // 2x combo
            gotIt(textureCache.handle("bonus/x2"), 2 * total);
        } else if(3 == combo) {
            // 3x combo
            gotIt(textureCache.handle("bonus/x3"), 3 * total);
        } else if(4 == combo) {
            // 4x combo
            gotIt(textureCache.handle("bonus/x4"), 4 * total);
        } else if(5 < combo) {
            // 5x combo
            gotIt(textureCache.handle("bonus/x5"), 5 * total);
        }

        dirty = false;
    } else {
        dirty = false;
    }
}


}
