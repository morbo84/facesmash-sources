#include "../common/util.h"
#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "face_button_system.h"


namespace gamee {


FaceButtonSystem::FaceButtonSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
}


FaceButtonSystem::~FaceButtonSystem() {
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


void FaceButtonSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void FaceButtonSystem::update(Registry &registry) {
    if(dirty) {
        auto view = registry.view<FaceButton, Transform, BoundingBox>();

        view.each([&, this](auto entity, auto &button, auto &transform, auto &box) {
            auto area = transformToPosition(registry, entity, transform) * box;

            if(button.enabled && SDL_PointInRect(&coord, &area)) {
                Locator::Dispatcher::ref().enqueue<FaceRequest>(button.type);
            }
        });

        dirty = false;
    }
}


}
