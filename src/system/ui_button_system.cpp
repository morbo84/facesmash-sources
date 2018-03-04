#include "../common/util.h"
#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "../service/audio_service.h"
#include "../service/av_recorder_android.h"
#include "../service/av_recorder_null.h"
#include "../service/game_services_service.h"
#include "ui_button_system.h"


namespace gamee {


namespace GameServicesStatus {


enum GameServicesStatus: Uint8 {
    SIGNED_IN = 2, // green icon
    SIGNED_OUT = 3, // red icon
    PENDING = 1 // blue icon
};


} // namespace detail


static void handleUserButtonClick(Registry &registry) {
    auto view = registry.view<UIButton, Sprite>();

    for(auto entity: view) {
        auto &sprite = registry.get<Sprite>(entity);

        switch(sprite.frame){
            case GameServicesStatus::SIGNED_IN:
                Locator::GameServices::ref().signOut();
                break;
            case GameServicesStatus::SIGNED_OUT:
                Locator::GameServices::ref().signIn();
                break;
            case GameServicesStatus::PENDING:
                // do nothing
                break;
        }
    }
}


static void switchAudio(Registry &registry, entity_type button) {
    // TODO
}


static void switchVideo(Registry &registry, entity_type button) {
    // TODO
}


UIButtonSystem::UIButtonSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
    Locator::Dispatcher::ref().connect<GameServicesEvent>(this);
}


UIButtonSystem::~UIButtonSystem() {
    Locator::Dispatcher::ref().disconnect<GameServicesEvent>(this);
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


void UIButtonSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void UIButtonSystem::receive(const GameServicesEvent &event) noexcept {
    gsEvent.emplace(event);
}


void UIButtonSystem::update(Registry &registry) {
    if(gsEvent) {
        auto view = registry.view<UIButton, Sprite>();

        view.each([this](auto, auto &button, auto &sprite) {
            if(button.action == UIAction::LOGIN) {
                switch(gsEvent->type) {
                case GameServicesEvent::Type::SIGNED_IN:
                    sprite.frame = GameServicesStatus::SIGNED_IN;
                    break;
                case GameServicesEvent::Type::SIGNED_OUT:
                    sprite.frame = GameServicesStatus::SIGNED_OUT;
                    break;
                case GameServicesEvent::Type::SIGNING_IN:
                case GameServicesEvent::Type::SIGNING_OUT:
                    sprite.frame = GameServicesStatus::PENDING;
                    break;
                }
            }
        });

        gsEvent.reset();
    }

    if(dirty) {
        auto view = registry.view<UIButton, Transform, BoundingBox>();

        view.each([&, this](auto entity, auto &button, auto &transform, auto &box) {
            auto &dispatcher = Locator::Dispatcher::ref();
            auto &gservices = Locator::GameServices::ref();

            auto area = transformToPosition(registry, entity, transform) * box;

            if(button.enabled && SDL_PointInRect(&coord, &area)) {
                switch(button.action) {
                case UIAction::EASTER_EGG:
                    dispatcher.enqueue<ActivateEasterEggEvent>();
                    break;
                case UIAction::EXIT:
                    dispatcher.enqueue<EnvEvent>(EnvEvent::Type::TERMINATING);
                    break;
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
                case UIAction::MENU_CLOSE_UP:
                case UIAction::MENU_CLOSE_DOWN:
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
                    gservices.isSignedIn()
                            ? gservices.achievements().showAllUI()
                            : dispatcher.enqueue<SceneChangeEvent>(SceneType::LOGIN_PLEASE);
                    break;
                case UIAction::LEADERBOARD:
                    gservices.isSignedIn()
                            ? Locator::GameServices::ref().leaderboards().showAllLeaderboardsUI()
                            : dispatcher.enqueue<SceneChangeEvent>(SceneType::LOGIN_PLEASE);
                    break;
                case UIAction::SAVE:
                    Locator::AvRecorder::ref().exportMedia();
                    break;
                case UIAction::SWITCH_AUDIO:
                    switchAudio(registry, entity);
                    break;
                case UIAction::SWITCH_VIDEO:
                    switchVideo(registry, entity);
                    break;
                case UIAction::LOGIN:
                    handleUserButtonClick(registry);
                    break;
                case UIAction::BUY:
                    // TODO
                    break;
                case UIAction::GPG_LICENSE:
                    // TODO
                    break;
                }
            }
        });

        dirty = false;
    }
}


}
