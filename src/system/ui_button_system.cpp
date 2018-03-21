#include "../common/ease.h"
#include "../common/util.h"
#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "../service/game_services_service.h"
#include "../service/haptic_null.h"
#include "../service/haptic_sdl.h"
#include "ui_button_system.h"


namespace gamee {

#ifdef __ANDROID__
void showOssLicenses();
#else
void showOssLicenses() {}
#endif


static void switchAudio(Registry &registry, entity_type button) {
    auto &settings = gamee::Locator::Settings::ref();
    auto &dispatcher = Locator::Dispatcher::ref();
    const bool audio = settings.read("audio/available", true);

    settings.write("audio/available", !audio);

    if(audio) {
        dispatcher.enqueue<AudioEvent>(AudioEvent::Type::STOP);
        registry.get<Sprite>(button).frame = 3;
    } else {
        dispatcher.enqueue<AudioEvent>(AudioEvent::Type::START);
        registry.get<Sprite>(button).frame = 2;
    }
}


static void switchVideo(Registry &registry, entity_type button) {
    auto &settings = gamee::Locator::Settings::ref();
    auto &dispatcher = Locator::Dispatcher::ref();

#if __ANDROID__
    const bool video = settings.read("video/available", true);

    settings.write("video/available", !video);

    if(video) {
        dispatcher.enqueue<AvRecorderEvent>(AvRecorderEvent::Type::DISABLE);
        registry.get<Sprite>(button).frame = 3;
    } else {
        dispatcher.enqueue<AvRecorderEvent>(AvRecorderEvent::Type::ENABLE);
        registry.get<Sprite>(button).frame = 2;
    }
#else
    settings.write("video/available", false);
    dispatcher.enqueue<AvRecorderEvent>(AvRecorderEvent::Type::DISABLE);
    registry.get<Sprite>(button).frame = 3;
#endif
}


static void switchHaptic(Registry &registry, entity_type button) {
    auto &settings = gamee::Locator::Settings::ref();
    const bool haptic = settings.read("haptic/available", true);

    settings.write("haptic/available", !haptic);

    if(haptic) {
        Locator::Haptic::set<HapticNull>();
        registry.get<Sprite>(button).frame = 3;
    } else {
        Locator::Haptic::set<HapticSDL>();
        registry.get<Sprite>(button).frame = 2;
    }
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
                    sprite.frame = 2;
                    break;
                case GameServicesEvent::Type::SIGNED_OUT:
                    sprite.frame = 3;
                    break;
                case GameServicesEvent::Type::SIGNING_IN:
                case GameServicesEvent::Type::SIGNING_OUT:
                    sprite.frame = 1;
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
            auto &permissions = Locator::Permissions::ref();

            auto area = transformToPosition(registry, entity, transform) * box;

            if(registry.has<InputReceiver>(entity) && SDL_PointInRect(&coord, &area)) {
                registry.accomodate<RotationAnimation>(entity, 0.f, 360.f, 1500_ui32, 0_ui32, false, &easeOutElastic);

                switch(button.action) {
                case UIAction::EASTER_EGG:
                    dispatcher.enqueue<ActivateEasterEggEvent>();
                    registry.remove<RotationAnimation>(entity);
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
                case UIAction::CAMERA_PERMISSION:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
                    permissions.request(PermissionType::CAMERA);
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
                    dispatcher.enqueue<AvRecorderEvent>(AvRecorderEvent::Type::EXPORT);
                    registry.remove<InputReceiver>(entity);
                    registry.get<Sprite>(entity).frame = 3;
                    break;
                case UIAction::STORAGE_PERMISSION:
                    permissions.request(PermissionType::STORAGE);
                    registry.remove<RotationAnimation>(entity);
                    break;
                case UIAction::SWITCH_AUDIO:
                    switchAudio(registry, entity);
                    break;
                case UIAction::SWITCH_VIDEO:
                    switchVideo(registry, entity);
                    break;
                case UIAction::SWITCH_HAPTIC:
                    switchHaptic(registry, entity);
                    break;
                case UIAction::LOGIN:
                    Locator::GameServices::ref().isSignedIn()
                            ? Locator::GameServices::ref().signOut()
                            : Locator::GameServices::ref().signIn();
                    break;
                case UIAction::BUY:
                    // TODO
                    break;
                case UIAction::GPG_LICENSE:
                    showOssLicenses();
                    break;
                }
            }
        });

        dirty = false;
    }
}


}
