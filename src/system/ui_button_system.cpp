#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "ui_button_system.h"


namespace gamee {


UIButtonSystem::UIButtonSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
}


UIButtonSystem::~UIButtonSystem() {
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


void UIButtonSystem::receive(const TouchEvent &event) noexcept {
    coord.x = event.x;
    coord.y = event.y;
    dirty = true;
}


void UIButtonSystem::update(Registry &registry) {
    if(dirty) {
        auto view = registry.view<UIButton, Transform, BoundingBox>();

        view.each([&, this](auto, auto &button, auto &transform, auto &box) {
            auto area = transform * box;

            if(SDL_PointInRect(&coord, &area)) {
                if(registry.has<SceneChangeRequest>()) {
                    registry.destroy(registry.attachee<SceneChangeRequest>());
                }

                switch(button.action) {
                case UIButton::Action::CHALLENGE:
                    registry.attach<SceneChangeRequest>(registry.create(), SceneType::GAME_CHALLENGE);
                    break;
                case UIButton::Action::TRAINING:
                    registry.attach<SceneChangeRequest>(registry.create(), SceneType::GAME_TRAINING);
                    break;
                case UIButton::Action::TIMER:
                    registry.attach<SceneChangeRequest>(registry.create(), SceneType::GAME_TIMER);
                    break;
                }
            }
        });

        dirty = false;
    }
}


}
