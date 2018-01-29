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


void SceneSystem::discardExpiringContents(Registry &registry) {
    auto view = registry.view<ExpiringContent>();

    for(auto entity: view) {
        registry.destroy(entity);
    }
}


void SceneSystem::discardSplashScreen(Registry &registry) {
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


void SceneSystem::disableUIControls(Registry &registry) {
    for(auto entity: registry.view<UIButton, BoundingBox>()) {
        registry.remove<BoundingBox>(entity);
    }
}


void SceneSystem::enableUIControls(Registry &registry) {
    for(auto entity: registry.view<UIButton, Sprite>()) {
        const auto &sprite = registry.get<Sprite>(entity);
        registry.assign<BoundingBox>(entity, sprite.w, sprite.h);
    }
}


void SceneSystem::disableCameraFrame(Registry &registry) {
    if(registry.has<CameraFrame>()) {
        registry.get<Renderable>(registry.attachee<CameraFrame>()).alpha = 0;
        registry.get<CameraFrame>().acquire = false;
    }
}


void SceneSystem::enableCameraFrame(Registry &registry) {
    if(registry.has<CameraFrame>()) {
        registry.get<Renderable>(registry.attachee<CameraFrame>()).alpha = 255;
        registry.get<CameraFrame>().acquire = true;
    }
}


void SceneSystem::resetGame(Registry &registry) {
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


void SceneSystem::resetTraining(Registry &registry) {
    // TODO
}


delta_type SceneSystem::splashScreenTransition(Registry &registry) {
    static constexpr delta_type duration = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration / 3, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration / 3, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TUTORIAL_TOP:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::TRAINING_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
        case PanelType::TRAINING_BOTTOM:
        case PanelType::SMASH_BUTTONS:
            // they are already out of scene
            break;
        case PanelType::SPLASH_SCREEN:
            registry.accomodate<Transform>(entity, entity, 0.f, 0.f);
            break;
        }
    });

    return duration;
}


delta_type SceneSystem::menuPageTransition(Registry &registry) {
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
        case PanelType::TUTORIAL_TOP:
        case PanelType::TUTORIAL_BOTTOM:
            // they are already out of scene
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
            assert(false);
        }
    });

    return duration;
}


delta_type SceneSystem::creditsTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
        case PanelType::BACKGROUND_BOTTOM:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TUTORIAL_TOP:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
        case PanelType::SMASH_BUTTONS:
            // they are already out of scene
            break;
        case PanelType::CREDITS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            assert(false);
        }
    });

    // TODO

    return duration;
}


delta_type SceneSystem::supportTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
        case PanelType::BACKGROUND_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TUTORIAL_TOP:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
        case PanelType::SMASH_BUTTONS:
            // they are already out of scene
            break;
        case PanelType::SUPPORT:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            assert(false);
        }
    });

    // TODO

    return duration;
}


delta_type SceneSystem::settingsTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
        case PanelType::BACKGROUND_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TUTORIAL_TOP:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
        case PanelType::SMASH_BUTTONS:
            // they are already out of scene
            break;
        case PanelType::SETTINGS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            assert(false);
        }
    });

    // TODO

    return duration;
}


delta_type SceneSystem::achievementsTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
        case PanelType::BACKGROUND_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::TUTORIAL_TOP:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
        case PanelType::SMASH_BUTTONS:
            // they are already out of scene
            break;
        case PanelType::ACHIEVEMENTS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            assert(false);
        }
    });

    // TODO

    return duration;
}


delta_type SceneSystem::gameTutorialTransition(Registry &registry) {
    static constexpr delta_type duration = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
        case PanelType::BACKGROUND_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
        case PanelType::SMASH_BUTTONS:
            // they are already out of scene
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 3, 0_ui32, &easeInCubic);
            break;
        case PanelType::TUTORIAL_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration / 3, 0_ui32, &easeInCubic);
            break;
        default:
            assert(false);
        }
    });

    return duration;
}


delta_type SceneSystem::theGameTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
            // they are already out of scene
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
            assert(false);
        }
    });

    return duration;
}


delta_type SceneSystem::gameOverTransition(Registry &registry) {
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
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TUTORIAL_TOP:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
            // they should stay where they are
            break;
        case PanelType::GAME_OVER:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::SMASH_BUTTONS:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        default:
            assert(false);
        }
    });

    return duration;
}


delta_type SceneSystem::trainingTutorialTransition(Registry &registry) {
    static constexpr delta_type duration = 3000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
        case PanelType::BACKGROUND_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
        case PanelType::TRAINING_TOP:
        case PanelType::TRAINING_BOTTOM:
        case PanelType::SMASH_BUTTONS:
            // they are already out of scene
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 6, 0_ui32, &easeOutCubic);
            break;
        case PanelType::TUTORIAL_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration / 3, 0_ui32, &easeInCubic);
            break;
        default:
            assert(false);
        }
    });

    return duration;
}


delta_type SceneSystem::trainingTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - logicalHeight / 8), duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 7 * logicalHeight / 8, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_TOP:
        case PanelType::MENU_BOTTOM:
        case PanelType::CREDITS:
        case PanelType::SUPPORT:
        case PanelType::SETTINGS:
        case PanelType::ACHIEVEMENTS:
        case PanelType::TUTORIAL_BOTTOM:
        case PanelType::THE_GAME_TOP:
        case PanelType::THE_GAME_BOTTOM:
        case PanelType::GAME_OVER:
            // they are already out of scene
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
            assert(false);
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
        case SceneType::TRAINING_TUTORIAL:
        case SceneType::GAME_TUTORIAL:
        case SceneType::SPLASH_SCREEN:
            // back is not allowed in these scenes
            break;
        case SceneType::CREDITS_PAGE:
        case SceneType::SUPPORT_PAGE:
        case SceneType::SETTINGS_PAGE:
        case SceneType::ACHIEVEMENTS_PAGE:
        case SceneType::TRAINING:
        case SceneType::GAME_OVER:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
            break;
        case SceneType::MENU_PAGE:
            dispatcher.enqueue<EnvEvent>(EnvEvent::Type::TERMINATING);
            break;
        case SceneType::THE_GAME:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_OVER);
            break;
        default:
            assert(false);
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
                    disableCameraFrame(registry);
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
