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


void discardExpiringContents(Registry &registry) {
    auto view = registry.view<ExpiringContent>();

    for(auto entity: view) {
        registry.destroy(entity);
    }
}


void discardSplashScreen(Registry &registry) {
    // we can safely discard the splash screen once reached
    registry.view<Panel>().each([&registry](auto parent, const auto &panel) {
        if(panel.type == PanelType::SPLASH_SCREEN) {
            registry.view<Transform>().each([parent, &registry](auto child, const auto &transform) {
                if(child != parent && transform.parent == parent) {
                    registry.assign<DestroyLater>(child, 1000_ui32);
                    registry.get<Transform>(child).parent = child;
                }
            });

            registry.destroy(parent);
        }
    });
}


void disableUIControls(Registry &registry) {
    for(auto entity: registry.view<UIButton, BoundingBox>()) {
        registry.remove<BoundingBox>(entity);
    }
}


void enableUIControls(Registry &registry) {
    for(auto entity: registry.view<UIButton, Sprite>()) {
        const auto &sprite = registry.get<Sprite>(entity);
        registry.assign<BoundingBox>(entity, sprite.w, sprite.h);
    }
}


void disableCameraFrame(Registry &registry) {
    if(registry.has<CameraFrame>()) {
        registry.get<Renderable>(registry.attachee<CameraFrame>()).alpha = 0;
        registry.get<CameraFrame>().acquire = false;
    }
}


void enableCameraFrame(Registry &registry) {
    if(registry.has<CameraFrame>()) {
        registry.get<Renderable>(registry.attachee<CameraFrame>()).alpha = 255;
        registry.get<CameraFrame>().acquire = true;
    }
}


void resetGame(Registry &registry) {
    auto &playerScore = registry.get<PlayerScore>();
    auto oldPlayerScore = playerScore;
    playerScore = {};

    for(auto i = 0u; i < std::extent<decltype(PlayerScore::entities)>::value; ++i) {
        playerScore.entities[i] = oldPlayerScore.entities[i];
    }

    auto &gameTimer = registry.get<GameTimer>();
    auto oldGameTimer = gameTimer;
    gameTimer = {};

    for(auto i = 0u; i < std::extent<decltype(GameTimer::entities)>::value; ++i) {
        gameTimer.entities[i] = oldGameTimer.entities[i];
    }

    registry.attach<LetsPlay>(registry.create());
}


void resetTraining(Registry &registry) {
    // TODO
}


delta_type exitTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::MENU_TOP:
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h / 4, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_BOTTOM:
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - 3 * panel.h / 4, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::EXIT:
            registry.accomodate<Transform>(entity, entity, 0.f, 0.f);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


delta_type splashScreenTransition(Registry &registry) {
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


delta_type menuPageTransition(Registry &registry) {
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
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::SMASH_BUTTONS:
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


delta_type creditsTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::CREDITS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


delta_type supportTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::SUPPORT:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


delta_type settingsTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::SETTINGS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


delta_type achievementsTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            break;
        case PanelType::ACHIEVEMENTS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 2, 0_ui32, &easeOutCubic);
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


delta_type gameTutorialTransition(Registry &registry) {
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


delta_type theGameTransition(Registry &registry) {
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
        case PanelType::SMASH_BUTTONS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth - 3 * panel.w / 2, duration, 0_ui32, &easeOutElastic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


delta_type gameOverTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.destroy(registry.attachee<LetsPlay>());
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
        case PanelType::SMASH_BUTTONS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


delta_type trainingTutorialTransition(Registry &registry) {
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


delta_type trainingTransition(Registry &registry) {
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
        case PanelType::SMASH_BUTTONS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth - 3 * panel.w / 2, duration, 0_ui32, &easeOutElastic);
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
        case SceneType::ACHIEVEMENTS_PAGE:
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
        auto &avMuxer = Locator::AvMuxer::ref();
        auto &camera = Locator::Camera::ref();
        auto &ads = Locator::Ads::ref();

        if(isTransitioning) {
            remaining = delta > remaining ? 0_ui32 : (remaining - delta);

            if(!remaining) {
                switch(next) {
                case SceneType::EXIT:
                    enableUIControls(registry);
                    break;
                case SceneType::SPLASH_SCREEN:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
                    discardSplashScreen(registry);
                    ads.show(AdsType::BANNER);
                    break;
                case SceneType::CREDITS_PAGE:
                case SceneType::SUPPORT_PAGE:
                case SceneType::SETTINGS_PAGE:
                case SceneType::ACHIEVEMENTS_PAGE:
                    enableUIControls(registry);
                    break;
                case SceneType::MENU_PAGE:
                    enableUIControls(registry);
                    camera.stop();
                    break;
                case SceneType::GAME_TUTORIAL:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::THE_GAME);
                    break;
                case SceneType::THE_GAME:
                    ads.load(AdsType::INTERSTITIAL);
                    resetGame(registry);
                    break;
                case SceneType::GAME_OVER:
                    avMuxer.stop();
                    disableCameraFrame(registry);
                    ads.isLoaded(AdsType::INTERSTITIAL)
                            ? (ads.show(AdsType::INTERSTITIAL), enableUIControls(registry))
                            : enableUIControls(registry);
                    break;
                case SceneType::TRAINING_TUTORIAL:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING);
                    break;
                case SceneType::TRAINING:
                    resetTraining(registry);
                    break;
                default:
                    assert(false);
                    break;
                }

                isTransitioning = false;
                curr = next;
            }
        } else {
            disableUIControls(registry);

            switch(next) {
            case SceneType::EXIT:
                disableCameraFrame(registry);
                remaining = exitTransition(registry);
                break;
            case SceneType::SPLASH_SCREEN:
                remaining = splashScreenTransition(registry);
                break;
            case SceneType::CREDITS_PAGE:
                remaining = creditsTransition(registry);
                break;
            case SceneType::SUPPORT_PAGE:
                discardExpiringContents(registry);
                refreshSupportPanel(registry);
                remaining = supportTransition(registry);
                break;
            case SceneType::SETTINGS_PAGE:
                remaining = settingsTransition(registry);
                break;
            case SceneType::ACHIEVEMENTS_PAGE:
                discardExpiringContents(registry);
                refreshAchievementsPanel(registry);
                remaining = achievementsTransition(registry);
                break;
            case SceneType::MENU_PAGE:
                remaining = menuPageTransition(registry);
                break;
            case SceneType::GAME_TUTORIAL:
                camera.start();
                avMuxer.start(recordingWidth, recordingHeight);
                remaining = gameTutorialTransition(registry);
                break;
            case SceneType::THE_GAME:
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
