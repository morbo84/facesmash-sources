#include <cassert>
#include <algorithm>
#include "../common/constants.h"
#include "../common/ease.h"
#include "../component/component.hpp"
#include "../common/types.h"
#include "../event/event.hpp"
#include "../factory/common.h"
#include "../factory/game_factory.h"
#include "../factory/play_factory.h"
#include "../locator/locator.hpp"
#include "scene_system.h"


namespace gamee {


static void discardExpiringContents(Registry &registry) {
    auto view = registry.view<ExpiringContent>();

    for(auto entity: view) {
        registry.destroy(entity);
    }
}


static void hideBackgroundPanels(Registry &registry) {
    registry.view<Panel, Transform>().each([](auto, const auto &panel, auto &transform) {
        switch(panel.type) {
        case PanelType::EXIT:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::SPLASH_SCREEN:
        case PanelType::PLAY_GAME:
            transform.x = -panel.w;
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });
}


static void disableUIButtons(Registry &registry) {
    for(auto entity: registry.view<UIButton, InputReceiver>()) {
        registry.remove<InputReceiver>(entity);
    }
}


static void enableUIButtons(Registry &registry, PanelType type) {
    registry.view<Panel>().each([&registry, type](auto parent, const auto &panel) {
        if(panel.type == type) {
            registry.view<UIButton, Transform>().each([parent, &registry](auto child, auto &button, const auto &transform) {
                if(transform.parent == parent && button.enabled) {
                    registry.accommodate<InputReceiver>(child);
                }
            });
        }
    });
}


static void showSmashButtons(Registry &registry) {
    auto view = registry.view<SmashButton, Sprite, Transform>();

    for(auto entity: view) {
        const auto &sprite = registry.get<Sprite>(entity);
        const auto &transform = registry.get<Transform>(entity);
        registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth - 3 * sprite.w / 2, 500_ui32, 0_ui32, &easeInCubic);
    }
}


static void hideSmashButtons(Registry &registry) {
    auto view = registry.view<SmashButton, Transform>();

    for(auto entity: view) {
        const auto &transform = registry.get<Transform>(entity);
        registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, 500_ui32, 0_ui32, &easeInCubic);
    }
}


static void showPopupButtons(Registry &registry, PanelType type) {
    registry.view<Panel>().each([&registry, type](auto parent, const auto &panel) {
        if(panel.type == type) {
            registry.view<UIButton, Sprite, Renderable, Transform>().each([parent, &registry](auto entity, auto &button, const auto &sprite, const auto &renderable, const auto &transform) {
                if(transform.parent == parent && button.popup) {
                    registry.accommodate<RotationAnimation>(entity, 0.f, 720.f, 2000_ui32, 0_ui32, false, &easeOutElastic);
                    registry.accommodate<SizeAnimation>(entity, sprite.w, sprite.h, button.w, button.h, 1500_ui32, 0_ui32, &easeOutElastic);
                }
            });
        }
    });
}


static void hidePopupButtons(Registry &registry, PanelType type) {
    registry.view<Panel>().each([&registry, type](auto parent, const auto &panel) {
        if(panel.type == type) {
            registry.view<UIButton, Sprite, Renderable, Transform>().each([parent, &registry](auto entity, const auto &button, const auto &sprite, const auto &renderable, const auto &transform) {
                if(transform.parent == parent && button.popup) {
                    registry.accommodate<RotationAnimation>(entity, 0.f, 720.f, 2000_ui32, 0_ui32, false, &easeOutCubic);
                    registry.accommodate<SizeAnimation>(entity, sprite.w, sprite.h, 0, 0, 500_ui32, 0_ui32, &easeInCubic);
                }
            });
        }
    });
}


static void enableFaceButtons(Registry &registry) {
    registry.view<FaceButton, Renderable>().each([](auto, auto &button, auto &renderable) {
        button.enabled = true;
        renderable.angle = 0.f;
    });
}


static void disableFaceButtons(Registry &registry) {
    registry.view<FaceButton, Renderable>().each([](auto, auto &button, auto &renderable) {
        button.enabled = false;
        renderable.angle = 0.f;
    });
}


static void showFaceButtons(Registry &registry) {
    registry.view<FaceButton, Sprite, Renderable>().each([&registry](auto entity, auto &button, const auto &sprite, auto &renderable) {
        registry.accommodate<RotationAnimation>(entity, renderable.angle, 720.f, 1000_ui32, 0_ui32, false, &easeOutElastic);
        registry.accommodate<SizeAnimation>(entity, sprite.w, sprite.h, button.w, button.h, 1000_ui32, 0_ui32, &easeOutElastic);
    });
}


static void hideFaceButtons(Registry &registry) {
    registry.view<FaceButton, Sprite, Renderable>().each([&registry](auto entity, auto &button, const auto &sprite, auto &renderable) {
        registry.accommodate<RotationAnimation>(entity, renderable.angle, 720.f, 1000_ui32, 0_ui32, false, &easeOutCubic);
        registry.accommodate<SizeAnimation>(entity, sprite.w, sprite.h, 0, 0, 1000_ui32, 0_ui32, &easeInCubic);
    });
}


static void resetPulseButton(Registry &registry) {
    auto view = registry.view<UIButton, PulseAnimation>();

    for(auto entity: view) {
        registry.get<PulseAnimation>(entity).elapsed = 0_ui32;
    }
}


static void showShareMessage(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    const auto inviteHandle = textureCache.handle("str/share");
    auto inviteLabel = createLastingMessage(registry, inviteHandle, 200);
    const auto &inviteSprite = registry.get<Sprite>(inviteLabel);
    setPos(registry, inviteLabel, (logicalWidth - inviteSprite.w) / 2, 7 * logicalHeight / 8);
    registry.assign<ExpiringContent>(inviteLabel);
}


static void disableCameraFrame(Registry &registry) {
    if(registry.has<CameraFrame>()) {
        registry.get<Renderable>(registry.attachee<CameraFrame>()).alpha = 0;
        registry.get<CameraFrame>().acquire = false;
    }
}


static void enableCameraFrame(Registry &registry) {
    if(registry.has<CameraFrame>()) {
        registry.get<Renderable>(registry.attachee<CameraFrame>()).alpha = 255;
        registry.get<CameraFrame>().acquire = true;
    }
}


static void clearTheGame(Registry &registry) {
    registry.destroy(registry.attachee<LetsPlay>());
}


static void clearTraining(Registry &registry) {
    if(registry.has<LetsTrain>()) {
        registry.destroy(registry.attachee<LetsTrain>());
    }
}


static void clearEndless(Registry &registry) {
    registry.destroy(registry.attachee<LetsEndless>());
}


static void clearTetris(Registry &registry) {
    registry.destroy(registry.attachee<LetsTetris>());
}


static void initTheGame(Registry &registry) {
    auto game = registry.create();
    registry.assign<LetsPlay>(entt::tag_t{}, game);
    registry.assign<PlayerScore>(entt::tag_t{}, game);
    registry.assign<Timer>(entt::tag_t{}, game, gameDuration);
}


static void initTraining(Registry &registry) {
    auto game = registry.create();
    registry.assign<LetsTrain>(entt::tag_t{}, game);
    registry.assign<PlayerScore>(entt::tag_t{}, game);
}


static void initEndless(Registry &registry) {
    auto endless = registry.create();
    registry.assign<LetsEndless>(entt::tag_t{}, endless);
    // TODO
}


static void initTetris(Registry &registry) {
    auto tetris = registry.create();
    registry.assign<LetsEndless>(entt::tag_t{}, tetris);
    // TODO
}


static delta_type splashScreenTransition(Registry &registry) {
    static constexpr delta_type duration = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration / 3, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration / 3, 0_ui32, &easeOutElastic);
            break;
        case PanelType::SPLASH_SCREEN:
            registry.accommodate<Transform>(entity, entity, 0.f, 0.f);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type menuPageTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::GAME_OVER:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::THE_GAME_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TRAINING_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type bgPanelTransition(Registry &registry, PanelType type) {
    static constexpr delta_type duration = 1000_ui32;

    int offset;

    registry.view<Panel>().each([&registry, type, &offset](auto entity, const auto &panel) {
        if(panel.type == type) {
            registry.accommodate<Transform>(entity, entity, (logicalWidth - panel.w) / 2.f, (logicalHeight - panel.h) / 2.f);
            offset = panel.h / 2;
        }
    });

    registry.view<Panel, Transform>().each([&registry, offset](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::MENU_TOP:
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -offset, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_BOTTOM:
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h + offset, duration, 0_ui32, &easeOutElastic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type gameTutorialTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TITLE_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TITLE_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 6, 0_ui32, &easeOutCubic);
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 6, 0_ui32, &easeOutCubic);
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 3, 0_ui32, &easeInCubic);
            break;
        case PanelType::TUTORIAL_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 3, 0_ui32, &easeInCubic);
            break;
        case PanelType::GAME_OVER:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type theGameTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::TITLE_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration / 3, 0_ui32, &easeInCubic);
            break;
        case PanelType::TITLE_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration / 3, 0_ui32, &easeInCubic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TUTORIAL_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeInCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type gameOverTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;
    static constexpr delta_type transition = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::GAME_OVER:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    // used to suppress wrong warnings from g++
    (void)duration;

    return transition;
}


static delta_type trainingTutorialTransition(Registry &registry) {
    static constexpr delta_type duration = 1500_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 6, 0_ui32, &easeOutCubic);
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 6, 0_ui32, &easeOutCubic);
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 3, 0_ui32, &easeInCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type trainingSelectTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_LEFT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 2, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration / 2, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeInCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type trainingTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::TRAINING_LEFT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TRAINING_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth - panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TRAINING_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TRAINING_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


SceneSystem::SceneSystem()
    : curr{SceneType::UNKNOWN},
      next{SceneType::UNKNOWN},
      pending{SceneType::UNKNOWN},
      remaining{0_ui32},
      isTransitioning{false}
{
    Locator::Dispatcher::ref().sink<SceneChangeEvent>().connect(this);
    Locator::Dispatcher::ref().sink<KeyboardEvent>().connect(this);
    Locator::Dispatcher::ref().sink<PermissionEvent>().connect(this);
}


SceneSystem::~SceneSystem() {
    Locator::Dispatcher::ref().sink<PermissionEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<KeyboardEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<SceneChangeEvent>().disconnect(this);
}


void SceneSystem::receive(const SceneChangeEvent &event) noexcept {
    assert(!isTransitioning);

    pending = event.scene;

    // events are filtered out during transitions
    if(curr == next) {
        // forces camera permission page if required (game/training not allowed)
        if((Locator::Permissions::ref().status(PermissionType::CAMERA) != PermissionStatus::GRANTED)
                && (pending == SceneType::GAME_TUTORIAL || pending == SceneType::TRAINING_TUTORIAL))
        {
            Locator::Permissions::ref().request(PermissionType::CAMERA);
        } else {
            next = pending;
        }
    }
}


void SceneSystem::receive(const KeyboardEvent &event) noexcept {
    // inputs from keyboard are filtered out during transitions
    if(curr == next && event.type == KeyboardEvent::Type::BACK) {
        auto &dispatcher = Locator::Dispatcher::ref();

        switch(curr) {
        case SceneType::PLAY_PAGE:
        case SceneType::CREDITS_PAGE:
        case SceneType::SUPPORT_PAGE:
        case SceneType::SETTINGS_PAGE:
        case SceneType::TRAINING_SELECT:
        case SceneType::VIDEO_RECORDING_STOP:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
            break;
        case SceneType::TRAINING:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING_SELECT);
            break;
        case SceneType::MENU_PAGE:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::EXIT);
            break;
        case SceneType::EXIT:
            dispatcher.enqueue<QuitEvent>();
            break;
        case SceneType::THE_GAME:
            static constexpr auto forced = true;
            dispatcher.enqueue<TimeIsOverEvent>(forced);
            break;
        default:
            // back is not allowed in all the other scenes
            break;
        }
    }
}


void SceneSystem::receive(const PermissionEvent &event) noexcept {
    if(event.permission == PermissionType::CAMERA && event.result == PermissionStatus::GRANTED) {
        if(curr == next) {
            next = pending;
        }
    }
}


void SceneSystem::update(Registry &registry, delta_type delta) {
    if(curr != next) {
        auto &dispatcher = Locator::Dispatcher::ref();
        auto &avRecorder = Locator::AvRecorder::ref();
        auto &camera = Locator::Camera::ref();
        auto &ads = Locator::Ads::ref();
        auto &billing = Locator::Billing::ref();
        auto &gameServices = Locator::GameServices::ref();

        if(isTransitioning) {
            // tracks remaining before to update it (mainly for video recording purposes)
            const bool finished = !remaining;
            remaining -= std::min(remaining, delta);

            if(finished) {
                disableCameraFrame(registry);

                switch(next) {
                case SceneType::EXIT:
                    enableUIButtons(registry, PanelType::EXIT);
                    break;
                case SceneType::SPLASH_SCREEN:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
                    ads.show(AdsType::BANNER);
                    billing.queryPurchases();
                    gameServices.achievements().query(FaceSmashAchievement::LITTLE_SMASHER);
                    break;
                case SceneType::PLAY_PAGE:
                    enableUIButtons(registry, PanelType::BACKGROUND_BOTTOM);
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableUIButtons(registry, PanelType::PLAY_GAME);
                    break;
                case SceneType::CREDITS_PAGE:
                    enableUIButtons(registry, PanelType::BACKGROUND_BOTTOM);
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableUIButtons(registry, PanelType::CREDITS);
                    break;
                case SceneType::SUPPORT_PAGE:
                    enableUIButtons(registry, PanelType::BACKGROUND_BOTTOM);
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableUIButtons(registry, PanelType::SUPPORT);
                    break;
                case SceneType::SETTINGS_PAGE:
                    enableUIButtons(registry, PanelType::BACKGROUND_BOTTOM);
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableUIButtons(registry, PanelType::SETTINGS);
                    break;
                case SceneType::MENU_PAGE:
                    dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_RELAX, true);
                    hideBackgroundPanels(registry);
                    curr == SceneType::SPLASH_SCREEN ? showPopupButtons(registry, PanelType::MENU_BOTTOM) : void();
                    curr == SceneType::SPLASH_SCREEN ? showPopupButtons(registry, PanelType::MENU_TOP) : void();
                    camera.stop();
                    break;
                case SceneType::GAME_TUTORIAL:
                    dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_PLAY, true);
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::VIDEO_RECORDING_START);
                    hideBackgroundPanels(registry);
                    // starting the camera freezes the app, this seems to be a good point to risk a lag
                    camera.start();
                    break;
                case SceneType::VIDEO_RECORDING_START:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::THE_GAME);
                    break;
                case SceneType::VIDEO_RECORDING_STOP:
                    enableUIButtons(registry, PanelType::GAME_OVER);
                    break;
                case SceneType::THE_GAME:
                    enableCameraFrame(registry);
                    ads.load(AdsType::INTERSTITIAL);
                    initTheGame(registry);
                    break;
                case SceneType::GAME_OVER:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::VIDEO_RECORDING_STOP);
                    break;
                case SceneType::TRAINING_TUTORIAL:
                    dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_PLAY, true);
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING_SELECT);
                    hideBackgroundPanels(registry);
                    break;
                case SceneType::TRAINING_SELECT:
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableFaceButtons(registry);
                    enableCameraFrame(registry);
                    break;
                case SceneType::TRAINING:
                    enableCameraFrame(registry);
                    initTraining(registry);
                    break;
                case SceneType::ENDLESS_TUTORIAL:
                    // TODO
                    break;
                case SceneType::ENDLESS:
                    enableCameraFrame(registry);
                    ads.load(AdsType::INTERSTITIAL);
                    initEndless(registry);
                    break;
                case SceneType::TETRIS_TUTORIAL:
                    // TODO
                    break;
                case SceneType::TETRIS:
                    enableCameraFrame(registry);
                    ads.load(AdsType::INTERSTITIAL);
                    initTetris(registry);
                    break;
                default:
                    assert(false);
                    break;
                }

                enableUIButtons(registry, PanelType::MENU_BOTTOM);
                enableUIButtons(registry, PanelType::MENU_TOP);

                isTransitioning = false;
                curr = next;
            }
        } else {
            disableFaceButtons(registry);
            disableUIButtons(registry);
            hideFaceButtons(registry);
            hideSmashButtons(registry);
            hidePopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
            hidePopupButtons(registry, PanelType::BACKGROUND_TOP);
            hidePopupButtons(registry, PanelType::PLAY_GAME);
            hidePopupButtons(registry, PanelType::CREDITS);
            hidePopupButtons(registry, PanelType::SUPPORT);
            hidePopupButtons(registry, PanelType::SETTINGS);

            switch(next) {
            case SceneType::EXIT:
                remaining = bgPanelTransition(registry, PanelType::EXIT);
                break;
            case SceneType::SPLASH_SCREEN:
                dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_RELAX, true);
                enableUIButtons(registry, PanelType::SPLASH_SCREEN);
                hidePopupButtons(registry, PanelType::MENU_BOTTOM);
                hidePopupButtons(registry, PanelType::MENU_TOP);
                remaining = splashScreenTransition(registry);
                break;
            case SceneType::PLAY_PAGE:
                discardExpiringContents(registry);
                refreshPlayPanel(registry);
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::PLAY_GAME);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::PLAY_GAME);
                break;
            case SceneType::CREDITS_PAGE:
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::CREDITS);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::CREDITS);
                break;
            case SceneType::SUPPORT_PAGE:
                discardExpiringContents(registry);
                refreshSupportPanel(registry);
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::SUPPORT);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::SUPPORT);
                break;
            case SceneType::SETTINGS_PAGE:
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::SETTINGS);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::SETTINGS);
                break;
            case SceneType::MENU_PAGE:
                dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_RELAX, false);
                remaining = menuPageTransition(registry);
                break;
            case SceneType::GAME_TUTORIAL:
                dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_PLAY, false);
                dispatcher.enqueue<ArmageddonEvent>();
                discardExpiringContents(registry);
                remaining = gameTutorialTransition(registry);
                break;
            case SceneType::VIDEO_RECORDING_START:
                // video recording has a bootstrap time we want to manage to create better videos
                avRecorder.start(recordingWidth, recordingHeight);
                // read carefully the rules of the game!
                remaining = 3000_ui32;
                break;
            case SceneType::VIDEO_RECORDING_STOP:
                avRecorder.stop();
                showShareMessage(registry);
                showPopupButtons(registry, PanelType::GAME_OVER);
                ads.isLoaded(AdsType::INTERSTITIAL) ? ads.show(AdsType::INTERSTITIAL) : void();
                // we try to create enough room to finalize the video
                remaining = 1000_ui32;
                break;
            case SceneType::THE_GAME:
                showSmashButtons(registry);
                enableCameraFrame(registry);
                remaining = theGameTransition(registry);
                break;
            case SceneType::GAME_OVER:
                dispatcher.enqueue<ArmageddonEvent>();
                discardExpiringContents(registry);
                refreshGameOverPanel(registry);
                clearTheGame(registry);
                resetPulseButton(registry);
                hidePopupButtons(registry, PanelType::GAME_OVER);
                remaining = gameOverTransition(registry);
                break;
            case SceneType::TRAINING_TUTORIAL:
                dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_PLAY, false);
                camera.start();
                remaining = trainingTutorialTransition(registry);
                break;
            case SceneType::TRAINING_SELECT:
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showSmashButtons(registry);
                showFaceButtons(registry);
                clearTraining(registry);
                enableCameraFrame(registry);
                remaining = trainingSelectTransition(registry);
                break;
            case SceneType::TRAINING:
                showSmashButtons(registry);
                remaining = trainingTransition(registry);
                break;
            case SceneType::ENDLESS_TUTORIAL:
                // TODO
                break;
            case SceneType::ENDLESS:
                // TODO
                clearEndless(registry);
                break;
            case SceneType::TETRIS_TUTORIAL:
                // TODO
                break;
            case SceneType::TETRIS:
                // TODO
                clearTetris(registry);
                break;
            default:
                assert(false);
                break;
            }

            // make faces and items on screen (if any) no longer smashable
            registry.reset<Destroyable>();

            isTransitioning = true;
        }
    }
}


}
