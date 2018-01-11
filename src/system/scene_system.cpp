#include <cassert>
#include <algorithm>
#include "../common/constants.h"
#include "../common/ease.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "scene_system.h"


namespace gamee {


void SceneSystem::discardSplashScreen(Registry &registry) {
    // we can safely discard the splash screen once reached
    registry.view<Panel>().each([&registry](auto parent, const auto &panel) {
        if(panel.type == PanelType::SPLASH_SCREEN) {
            registry.view<Transform>().each([parent, &registry](auto child, const auto &transform) {
                if(child != parent && transform.parent == parent) {
                    registry.destroy(child);
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
        const auto handle = registry.get<Sprite>(entity).handle;
        registry.assign<BoundingBox>(entity, handle->width(), handle->height());
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


delta_type SceneSystem::menuPageTransition(Registry &registry, delta_type duration) {
    registry.view<Panel, Transform>().each([&registry, duration](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::TOP_PATCH:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BOTTOM_PATCH:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight / 2, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_TOP_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MENU_BOTTOM_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight / 2, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::GAME_TOP_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::GAME_BOTTOM_PANEL:
        case PanelType::GAME_OVER_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::SMASH_BUTTONS_PANEL:
#if DEBUG
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeInCubic);
#endif // DEBUG
            break;
        default:
            assert(false);
        }
    });

    return duration;
}


delta_type SceneSystem::theGameTransition(Registry &registry) {
    static constexpr delta_type transition = 1000_ui32;
    static constexpr delta_type duration = 3000_ui32;

    enableCameraFrame(registry);

    registry.get<Renderable>(registry.attachee<PlayButton>()).alpha = 0;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::TOP_PATCH:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -(panel.h - 320), transition, 0_ui32, &easeOutElastic);
            break;
        case PanelType::BOTTOM_PATCH:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - 320, transition, 0_ui32, &easeOutElastic);
            break;
        case PanelType::MENU_TOP_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, transition, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_BOTTOM_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, transition, 0_ui32, &easeInCubic);
            break;
        case PanelType::GAME_TOP_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, transition, 0_ui32, &easeOutCubic);
            break;
        case PanelType::GAME_BOTTOM_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight / 2, transition, 0_ui32, &easeOutCubic);
            break;
        case PanelType::GAME_OVER_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, transition, 0_ui32, &easeOutCubic);
            break;
        case PanelType::SMASH_BUTTONS_PANEL:
#if DEBUG
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth - 3 * panel.w / 2, transition / 2, 0_ui32, &easeOutCubic);
#endif // DEBUG
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

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::TOP_PATCH:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::BOTTOM_PATCH:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight / 2, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::MENU_TOP_PANEL:
        case PanelType::MENU_BOTTOM_PANEL:
            // they are already out of scene
            break;
        case PanelType::GAME_TOP_PANEL:
        case PanelType::GAME_BOTTOM_PANEL:
            // they should stay where they are
            break;
        case PanelType::GAME_OVER_PANEL:
            registry.accomodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight / 2 - panel.h / 2, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::SMASH_BUTTONS_PANEL:
#if DEBUG
            registry.accomodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeInCubic);
#endif // DEBUG
            break;
        default:
            assert(false);
        }
    });

    return duration;
}


void SceneSystem::splashScreen() {
    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
}


void SceneSystem::menuPage(Registry &registry) {
    registry.get<Renderable>(registry.attachee<PlayButton>()).alpha = 255;
}


void SceneSystem::theGame(Registry &registry) {
    registry.get<PlayerScore>() = {};
    registry.get<GameTimer>() = {};
    registry.attach<LetsPlay>(registry.create());
}


void SceneSystem::gameOver(Registry &registry) {
    disableCameraFrame(registry);
}


SceneSystem::SceneSystem()
    : curr{SceneType::UNKNOWN},
      next{SceneType::UNKNOWN},
      remaining{0_ui32},
      isTransitioning{false}
{
    Locator::Dispatcher::ref().connect<SceneChangeEvent>(this);
}


SceneSystem::~SceneSystem() {
    Locator::Dispatcher::ref().disconnect<SceneChangeEvent>(this);
}


void SceneSystem::receive(const SceneChangeEvent &event) noexcept {
    assert(!isTransitioning);
    next = event.scene;
}


void SceneSystem::update(Registry &registry, delta_type delta) {
    if(curr != next) {
        if(isTransitioning) {
            remaining = delta > remaining ? 0_ui32 : (remaining - delta);

            if(!remaining) {
                enableUIControls(registry);

                switch(next) {
                case SceneType::SPLASH_SCREEN:
                    splashScreen();
                    break;
                case SceneType::MENU_PAGE:
                    menuPage(registry);
                    break;
                case SceneType::THE_GAME:
                    theGame(registry);
                    break;
                case SceneType::GAME_OVER:
                    gameOver(registry);
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
                remaining = 3000_ui32;
                break;
            case SceneType::MENU_PAGE:
                remaining = (curr == SceneType::SPLASH_SCREEN)
                        ? (discardSplashScreen(registry), menuPageTransition(registry, 0_ui32))
                        : menuPageTransition(registry, 1000_ui32);
                break;
            case SceneType::THE_GAME:
                remaining = theGameTransition(registry);
                break;
            case SceneType::GAME_OVER:
                remaining = gameOverTransition(registry);
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
