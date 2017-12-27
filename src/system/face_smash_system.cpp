#include <SDL_rect.h>
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "../settings/settings.h"
#include "face_smash_system.h"


namespace gamee {


FaceSmashSystem::FaceSmashSystem()
    : type{SmashType::HAPPY},
      dirty{false}
{
    Locator::Dispatcher::ref().connect<SmashEvent>(this);
}


FaceSmashSystem::~FaceSmashSystem() {
    Locator::Dispatcher::ref().disconnect<SmashEvent>(this);
}


void FaceSmashSystem::receive(const SmashEvent &event) noexcept {
    type = event.type;
    dirty = true;
}


void FaceSmashSystem::update(Registry &registry, delta_type delta) {
    if(dirty) {
        auto view = registry.view<FaceSmash, Transform, BoundingBox>();
        Settings settings;

        SDL_Rect scene;
        scene.x = 0;
        scene.y = 0;
        scene.w = settings.logicalWidth();
        scene.h = settings.logicalHeight();

        view.each([&registry, delta, scene, this](auto entity, const auto &smash, const auto &transform, const auto &box) {
            if(smash.type == type) {
                // TODO particles

                // TODO match
            } else {
                const auto area = transform * box;

                if(!SDL_HasIntersection(&scene, &area)) {
                    // TODO miss
                }
            }
        });

        dirty = false;
    }
}


}
