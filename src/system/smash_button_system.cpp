#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "smash_button_system.h"


namespace gamee {


SmashButtonSystem::SmashButtonSystem()
    : generator{std::random_device{}()},
      dirty{false}
{
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
}


SmashButtonSystem::~SmashButtonSystem() {
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


void SmashButtonSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void SmashButtonSystem::update(Registry &registry) {
    if(dirty) {
        auto view = registry.view<SmashButton, Transform, BoundingBox>();

        view.each([&, this](auto entity, auto &button, auto &transform, auto &box) {
            auto area = transformToPosition(registry, entity, transform) * box;

            if(SDL_PointInRect(&coord, &area)) {
                std::uniform_real_distribution<> dis(probabilityThreshold, 1.f);
                float probability = dis(generator);
                Locator::Dispatcher::ref().enqueue<FaceEvent>(button.type, probability);
            }
        });

        dirty = false;
    }
}


}
