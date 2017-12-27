#include <SDL_rect.h>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
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
        const SDL_Rect screen = logicalScreen;

        view.each([&registry, delta, screen, this](auto entity, const auto &smash, const auto &transform, const auto &box) {
            if(smash.type == type) {
                // TODO particles

                // TODO match
            } else {
                const auto area = transform * box;

                if(!SDL_HasIntersection(&screen, &area)) {
                    // TODO miss
                }
            }
        });

        dirty = false;
    }
}


}
