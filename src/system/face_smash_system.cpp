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
    : type{SmashType::HAPPY},
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
    auto view = registry.view<FaceSmash, Transform, Movement, BoundingBox>();
    auto &textureCache = Locator::TextureCache::ref();
    const SDL_Rect screen = logicalScreen;

    ScoreEvent scoreEvent{ 0 };
    SmashEvent smashEvent{ 0_ui16, 0_ui16 };

    view.each([&, this](auto entity, const auto &smash, const auto &transform, const auto &movement, const auto &box) {
        const auto area = transform * box;
        SDL_Point center = { area.x + area.w / 2, area.y + area.h / 2 };

        if(movement.velY > 0 && !SDL_HasIntersection(&screen, &area)) {
            auto score = registry.create();
            registry.assign<Renderable>(score);
            registry.assign<Movement>(score, 0.f, 0.f, -.06f);
            registry.assign<FadeAnimation>(score, 255, 0, 2000_ui32);
            registry.assign<DestroyLater>(score, 2000_ui32);

            switch(smash.miss) {
            case 50:
                addScore(registry, textureCache.handle("miss/50"), center);
                break;
                // TODO other scores ...
            }

            scoreEvent.score -= smash.miss;
            ++smashEvent.miss;

            registry.destroy(entity);
        } else if(dirty && smash.type == type) {
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

            scoreEvent.score += smash.smash;
            ++smashEvent.smash;

            registry.destroy(entity);
        }
    });

    if(scoreEvent.score) {
        Locator::Dispatcher::ref().enqueue<ScoreEvent>(scoreEvent);
    }

    if(smashEvent.miss || smashEvent.smash) {
        Locator::Dispatcher::ref().enqueue<SmashEvent>(smashEvent);
    }

    dirty = false;
}


}
