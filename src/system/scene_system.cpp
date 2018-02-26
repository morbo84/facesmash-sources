#include <cassert>
#include <algorithm>
#include "../common/constants.h"
#include "../common/ease.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../factory/game_factory.h"
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
    registry.view<Panel, Transform>().each([&registry](auto, const auto &panel, auto &transform) {
        switch(panel.type) {
        case PanelType::EXIT:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::SPLASH_SCREEN:
            transform.x = -panel.w;
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });
}


static void disableUIButtons(Registry &registry) {
    for(auto entity: registry.view<UIButton>()) {
        registry.get<UIButton>(entity).enabled = false;
    }
}


static void enableUIButtons(Registry &registry, PanelType type) {
    registry.view<Panel>().each([&registry, type](auto parent, const auto &panel) {
        if(panel.type == type) {
            registry.view<UIButton, Transform>().each([parent, &registry](auto, auto &button, const auto &transform) {
                if(transform.parent == parent && !button.popup) {
                    button.enabled = true;
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
        registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth - 3 * sprite.w / 2, 500_ui32, 0_ui32, &easeInCubic);
    }
}


static void hideSmashButtons(Registry &registry) {
    auto view = registry.view<SmashButton, Transform>();

    for(auto entity: view) {
        const auto &transform = registry.get<Transform>(entity);
        registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, 500_ui32, 0_ui32, &easeInCubic);
    }
}


static void enableShowPopupButtons(Registry &registry, PanelType type) {
    registry.view<Panel>().each([&registry, type](auto parent, const auto &panel) {
        if(panel.type == type) {
            registry.view<UIButton, Sprite, Renderable, Transform>().each([parent, &registry](auto entity, auto &button, const auto &sprite, const auto &renderable, const auto &transform) {
                if(transform.parent == parent && button.popup) {
                    button.enabled = true;
                    registry.accomodate<RotationAnimation>(entity, renderable.angle, 720.f, 1500_ui32, 0_ui32, false, &easeOutElastic);
                    registry.accomodate<SizeAnimation>(entity, sprite.w, sprite.h, button.w, button.h, 1500_ui32, 0_ui32, &easeOutElastic);
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
                    registry.accomodate<RotationAnimation>(entity, renderable.angle, 0.f, 1500_ui32, 0_ui32, false, &easeOutCubic);
                    registry.accomodate<SizeAnimation>(entity, sprite.w, sprite.h, 0, 0, 500_ui32, 0_ui32, &easeInCubic);
                }
            });
        }
    });
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


static void clearGame(Registry &registry) {
    registry.destroy(registry.attachee<LetsPlay>());
}


static void clearTraining(Registry &registry) {
    if(registry.has<LetsTrain>()) {
        registry.destroy(registry.attachee<LetsTrain>());
    }
}


static void initGame(Registry &registry) {
    auto game = registry.create();
    registry.attach<LetsPlay>(game);
    registry.attach<PlayerScore>(game);
    registry.attach<Timer>(game, gameDuration);
}


static void initTraining(Registry &registry) {
    auto game = registry.create();
    registry.attach<LetsTrain>(game);
    registry.attach<Timer>(game);
}


static delta_type splashScreenTransition(Registry &registry) {
    static constexpr delta_type duration = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration / 3, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration / 3, 0_ui32, &easeOutElastic);
            break;
        case PanelType::SPLASH_SCREEN:
            registry.accomodate<Transform>(entity, entity, 0.f, 0.f);
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
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::GAME_OVER:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::THE_GAME_TOP:
        case PanelType::TRAINING_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::TRAINING_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
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

    registry.view<Panel, Transform>().each([&registry, type, &offset](auto entity, const auto &panel, const auto &transform) {
        if(panel.type == type) {
            registry.accomodate<Transform>(entity, entity, (logicalWidth - panel.w) / 2.f, (logicalHeight - panel.h) / 2.f);
            offset = panel.h / 2;
        }
    });

    registry.view<Panel, Transform>().each([&registry, type, offset](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::MENU_TOP:
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -offset, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_BOTTOM:
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h + offset, duration, 0_ui32, &easeOutElastic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type gameTutorialTransition(Registry &registry) {
    static constexpr delta_type duration = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 6, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 6, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 3, 0_ui32, &easeInCubic);
            break;
        case PanelType::TUTORIAL_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 3, 0_ui32, &easeInCubic);
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
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TUTORIAL_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::GAME_OVER:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
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

    registry.reset<Destroyable>();

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::GAME_OVER:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_TOP:
        case PanelType::TRAINING_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::TRAINING_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type trainingTutorialTransition(Registry &registry) {
    static constexpr delta_type duration = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 6, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 6, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 3, 0_ui32, &easeInCubic);
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
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::TRAINING_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeInCubic);
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
      remaining{0_ui32},
      isTransitioning{false}
{
    Locator::Dispatcher::ref().connect<SceneChangeEvent>(this);
    Locator::Dispatcher::ref().connect<KeyboardEvent>(this);
}


SceneSystem::~SceneSystem() {
    Locator::Dispatcher::ref().disconnect<KeyboardEvent>(this);
    Locator::Dispatcher::ref().disconnect<SceneChangeEvent>(this);
}


void SceneSystem::receive(const SceneChangeEvent &event) noexcept {
    assert(!isTransitioning);

    // events are filtered out during transitions
    if(curr == next) {
        next = event.scene;
    }
}


void SceneSystem::receive(const KeyboardEvent &event) noexcept {
    // inputs from keyboard are filtered out during transitions
    if(curr == next && event.type == KeyboardEvent::Type::BACK) {
        auto &dispatcher = Locator::Dispatcher::ref();

        switch(curr) {
        case SceneType::CREDITS_PAGE:
        case SceneType::SUPPORT_PAGE:
        case SceneType::SETTINGS_PAGE:
        case SceneType::TRAINING:
        case SceneType::GAME_OVER:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
            break;
        case SceneType::MENU_PAGE:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::EXIT);
            break;
        case SceneType::EXIT:
            dispatcher.enqueue<EnvEvent>(EnvEvent::Type::TERMINATING);
            break;
        case SceneType::THE_GAME:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_OVER);
            break;
        default:
            // back is not allowed in all the other scenes
            break;
        }
    }
}


void SceneSystem::update(Registry &registry, delta_type delta) {
    if(curr != next) {
        auto &dispatcher = Locator::Dispatcher::ref();
        auto &avRecorder = Locator::AvRecorder::ref();
        auto &camera = Locator::Camera::ref();
        auto &ads = Locator::Ads::ref();

        if(isTransitioning) {
            remaining = delta > remaining ? 0_ui32 : (remaining - delta);

            if(!remaining) {
                disableCameraFrame(registry);

                switch(next) {
                case SceneType::EXIT:
                    enableShowPopupButtons(registry, PanelType::MENU_BOTTOM);
                    enableShowPopupButtons(registry, PanelType::MENU_TOP);
                    enableUIButtons(registry, PanelType::EXIT);
                    break;
                case SceneType::SPLASH_SCREEN:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
                    ads.show(AdsType::BANNER);
                    break;
                case SceneType::CREDITS_PAGE:
                    enableUIButtons(registry, PanelType::CREDITS);
                    break;
                case SceneType::SUPPORT_PAGE:
                    enableUIButtons(registry, PanelType::SUPPORT);
                    break;
                case SceneType::SETTINGS_PAGE:
                    enableUIButtons(registry, PanelType::SETTINGS);
                    break;
                case SceneType::MENU_PAGE:
                    clearTraining(registry);
                    hideBackgroundPanels(registry);
                    enableShowPopupButtons(registry, PanelType::MENU_BOTTOM);
                    enableShowPopupButtons(registry, PanelType::MENU_TOP);
                    camera.stop();
                    break;
                case SceneType::GAME_TUTORIAL:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::THE_GAME);
                    break;
                case SceneType::THE_GAME:
                    ads.load(AdsType::INTERSTITIAL);
                    enableCameraFrame(registry);
                    initGame(registry);
                    break;
                case SceneType::GAME_OVER:
                    avRecorder.stop();
                    clearGame(registry);
                    ads.isLoaded(AdsType::INTERSTITIAL)
                            ? (ads.show(AdsType::INTERSTITIAL), enableUIButtons(registry, PanelType::GAME_OVER))
                            : enableUIButtons(registry, PanelType::GAME_OVER);
                    break;
                case SceneType::TRAINING_TUTORIAL:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING);
                    break;
                case SceneType::TRAINING:
                    enableCameraFrame(registry);
                    initTraining(registry);
                    break;
                default:
                    assert(false);
                    break;
                }

                isTransitioning = false;
                curr = next;
            }
        } else {
            disableUIButtons(registry);
            hideSmashButtons(registry);

            switch(next) {
            case SceneType::EXIT:
                remaining = bgPanelTransition(registry, PanelType::EXIT);
                break;
            case SceneType::SPLASH_SCREEN:
                enableUIButtons(registry, PanelType::SPLASH_SCREEN);
                hidePopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                hidePopupButtons(registry, PanelType::BACKGROUND_TOP);
                hidePopupButtons(registry, PanelType::MENU_BOTTOM);
                hidePopupButtons(registry, PanelType::MENU_TOP);
                remaining = splashScreenTransition(registry);
                break;
            case SceneType::CREDITS_PAGE:
                enableShowPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                enableShowPopupButtons(registry, PanelType::BACKGROUND_TOP);
                enableShowPopupButtons(registry, PanelType::CREDITS);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::CREDITS);
                break;
            case SceneType::SUPPORT_PAGE:
                discardExpiringContents(registry);
                refreshSupportPanel(registry);
                enableShowPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                enableShowPopupButtons(registry, PanelType::BACKGROUND_TOP);
                enableShowPopupButtons(registry, PanelType::SUPPORT);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::SUPPORT);
                break;
            case SceneType::SETTINGS_PAGE:
                enableShowPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                enableShowPopupButtons(registry, PanelType::BACKGROUND_TOP);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::SETTINGS);
                break;
            case SceneType::MENU_PAGE:
                hidePopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                hidePopupButtons(registry, PanelType::BACKGROUND_TOP);
                hidePopupButtons(registry, PanelType::CREDITS);
                hidePopupButtons(registry, PanelType::SUPPORT);
                remaining = menuPageTransition(registry);
                break;
            case SceneType::GAME_TUTORIAL:
                camera.start();
                avRecorder.start(recordingWidth, recordingHeight);
                remaining = gameTutorialTransition(registry);
                break;
            case SceneType::THE_GAME:
                showSmashButtons(registry);
                enableCameraFrame(registry);
                remaining = theGameTransition(registry);
                break;
            case SceneType::GAME_OVER:
                discardExpiringContents(registry);
                refreshGameOverPanel(registry);
                remaining = gameOverTransition(registry);
                break;
            case SceneType::TRAINING_TUTORIAL:
                camera.start();
                remaining = trainingTutorialTransition(registry);
                break;
            case SceneType::TRAINING:
                enableShowPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showSmashButtons(registry);
                enableCameraFrame(registry);
                remaining = trainingTransition(registry);
                break;
            default:
                assert(false);
                break;
            }

            isTransitioning = true;
        }
    }
}


}
