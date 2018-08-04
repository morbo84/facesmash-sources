#include "../common/ease.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
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
        registry.get<Sprite>(button).frame = 1;
    } else {
        dispatcher.enqueue<AudioEvent>(AudioEvent::Type::START);
        registry.get<Sprite>(button).frame = 0;
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
        registry.get<Sprite>(button).frame = 1;
    } else {
        dispatcher.enqueue<AvRecorderEvent>(AvRecorderEvent::Type::ENABLE);
        registry.get<Sprite>(button).frame = 0;
    }
#else
    settings.write("video/available", false);
    dispatcher.enqueue<AvRecorderEvent>(AvRecorderEvent::Type::DISABLE);
    registry.get<Sprite>(button).frame = 1;
#endif
}


static void switchHaptic(Registry &registry, entity_type button) {
    auto &settings = gamee::Locator::Settings::ref();
    const bool haptic = settings.read("haptic/available", true);

    settings.write("haptic/available", !haptic);

    if(haptic) {
        Locator::Haptic::set<HapticNull>();
        registry.get<Sprite>(button).frame = 1;
    } else {
        Locator::Haptic::set<HapticSDL>();
        registry.get<Sprite>(button).frame = 0;
    }
}


UIButtonSystem::UIButtonSystem(): dirty{false}, pending{nullptr} {
    Locator::Dispatcher::ref().sink<TouchEvent>().connect(this);
}


UIButtonSystem::~UIButtonSystem() {
    Locator::Dispatcher::ref().sink<TouchEvent>().disconnect(this);
}


void UIButtonSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void UIButtonSystem::showAchievements() {
    Locator::GameServices::ref().achievements().showAllUI();
}


void UIButtonSystem::showLeaderboard() {
    Locator::GameServices::ref().leaderboards().showAllLeaderboardsUI();
}


void UIButtonSystem::updateLoginButton(Registry &registry) {
    const auto &gameServices = Locator::GameServices::ref();
    auto view = registry.view<UIButton, Sprite>();
    Uint8 frame{};

    switch(gameServices.status()) {
        case GameServicesService::Status ::SIGNED_IN:
            pending = pending ? ((this->*pending)(), nullptr) : nullptr;
            frame = 0;
            break;
        case GameServicesService::Status ::SIGNED_OUT:
            pending = nullptr;
            frame = 1;
            break;
        case GameServicesService::Status ::SIGNING_IN:
            frame = 0;
            break;
        case GameServicesService::Status ::SIGNING_OUT:
            frame = 1;
            break;
    }

    view.each([frame](auto, auto &button, auto &sprite) {
        if(button.action == UIAction::LOGIN) {
            sprite.frame = frame;
        }
    });
}


void UIButtonSystem::update(Registry &registry) {
    updateLoginButton(registry);

    if(dirty) {
        auto view = registry.view<UIButton, Transform, BoundingBox>();

        view.each([&, this](auto entity, auto &button, auto &transform, auto &box) {
            auto area = transformToPosition(registry, entity, transform) * box;

            if(registry.has<InputReceiver>(entity) && SDL_PointInRect(&coord, &area)) {
                auto &dispatcher = Locator::Dispatcher::ref();
                auto &gservices = Locator::GameServices::ref();
                auto &haptic = Locator::Haptic::ref();
                auto &billing = Locator::Billing::ref();

                registry.accommodate<RotationAnimation>(entity, 0.f, 360.f, 1500_ui32, 0_ui32, false, &easeOutElastic);
                haptic.rumble(RumbleEffect::SUPER_SUPER_SOFT);

                switch(button.action) {
                case UIAction::EASTER_EGG:
                    dispatcher.enqueue<ActivateEasterEggEvent>();
                    registry.remove<RotationAnimation>(entity);
                    break;
                case UIAction::EXIT:
                    dispatcher.enqueue<QuitEvent>();
                    break;
                case UIAction::THE_GAME:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_TUTORIAL);
                    break;
                case UIAction::TRAINING:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING_TUTORIAL);
                    break;
                case UIAction::RESTART:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_TUTORIAL);
                    break;
                case UIAction::MENU:
                case UIAction::CLOSE:
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
                            ? showAchievements()
                            : (pending = &UIButtonSystem::showAchievements, Locator::GameServices::ref().signIn());
                    break;
                case UIAction::LEADERBOARD:
                    gservices.isSignedIn()
                            ? showLeaderboard()
                            : (pending = &UIButtonSystem::showLeaderboard, Locator::GameServices::ref().signIn());
                    break;
                case UIAction::SHARE:
                    dispatcher.enqueue<AvRecorderEvent>(AvRecorderEvent::Type::EXPORT);
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
                case UIAction::SHOP:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
                    billing.performPurchase(Product::REMOVE_ADS);
                    break;
                case UIAction::GPG_LICENSE:
                    showOssLicenses();
                    break;
                case UIAction::LOCKED:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::SUPPORT_PAGE);
                    break;
                case UIAction::MORE:
                    // TODO
                    break;
                }
            }
        });

        dirty = false;
    }
}


}
