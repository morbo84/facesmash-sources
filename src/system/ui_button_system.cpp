#include "../common/util.h"
#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "../service/audio_null.h"
#include "../service/audio_sdl.h"
#include "ui_button_system.h"


namespace gamee {


UIButtonSystem::UIButtonSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
}


UIButtonSystem::~UIButtonSystem() {
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


void UIButtonSystem::switchAudio(Registry &registry, entity_type entity) {
    return Locator::Audio::ref().isMute()
            ? Locator::Audio::set<AudioSdl>()
            : Locator::Audio::set<AudioNull>();
}


void UIButtonSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void UIButtonSystem::update(Registry &registry) {
    if(dirty) {
        auto view = registry.view<UIButton, Transform, BoundingBox>();

        view.each([&, this](auto entity, auto &button, auto &transform, auto &box) {
            auto &dispatcher = Locator::Dispatcher::ref();

            auto area = transformToPosition(registry, entity, transform) * box;

            if(SDL_PointInRect(&coord, &area)) {
                switch(button.action) {
                case UIAction::THE_GAME:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_TUTORIAL);
                    break;
                case UIAction::TRAINING:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING_TUTORIAL);
                    break;
                case UIAction::RELOAD:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::THE_GAME);
                    break;
                case UIAction::MENU:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
                    break;
                case UIAction::CREDITS:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::CREDITS_PAGE);
                    break;
                case UIAction::SUPPORT:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::SUPPORT_PAGE);
                    break;
                case UIAction::SETTINGS:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::SETTINGS_PAGE);
                    break;
                case UIAction::ACHIEVEMENTS:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::ACHIEVEMENTS_PAGE);
                    break;
                case UIAction::SAVE:
                    // TODO
                    break;
                case UIAction::SWITCH_AUDIO:
                    switchAudio(registry, entity);
                    break;
                }
            }
        });

        dirty = false;
    }
}


}
