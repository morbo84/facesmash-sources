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


#ifdef __ANDROID__
// prompt the user to rate the app in the store
void showRateDialog() noexcept;
#else
void showRateDialog() noexcept {}
#endif


static void enableStanza(Registry &registry, StanzaType type) {
    registry.view<Stanza>().each([type](const auto, auto &stanza) {
        if(stanza.type == type) {
            stanza.enabled = true;
        }
    });
}


static void disableStanza(Registry &registry) {
    registry.view<Stanza>().each([](const auto, auto &stanza) {
        stanza.enabled = false;
    });
}


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
        case PanelType::INFO:
        case PanelType::WALLPAPER:
        case PanelType::MULTIPLAYER:
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
            registry.view<UIButton, Sprite, Renderable, Transform>().each([parent, &registry](auto entity, auto &button, const auto &sprite, const auto &, const auto &transform) {
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
            registry.view<UIButton, Sprite, Renderable, Transform>().each([parent, &registry](auto entity, const auto &button, const auto &sprite, const auto &, const auto &transform) {
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
    registry.view<FaceButton, Sprite, Renderable>().each([&registry](auto entity, auto &, const auto &sprite, auto &renderable) {
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


static void showShareMessages(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    const auto hallOfFacesHandle = textureCache.handle("str/share/halloffaces");
    auto topLabel = createLastingMessage(registry, hallOfFacesHandle, 200);
    const auto &topSprite = registry.get<Sprite>(topLabel);
    setPos(registry, topLabel, (logicalWidth - topSprite.w) / 2, logicalHeight / 8);
    registry.assign<ExpiringContent>(topLabel);

    const auto faceSmashHandle = textureCache.handle("str/share/facesmash");
    auto bottomLabel = createLastingMessage(registry, faceSmashHandle, 200);
    const auto &bottomSprite = registry.get<Sprite>(bottomLabel);
    setPos(registry, bottomLabel, (logicalWidth - bottomSprite.w) / 2, 3 * logicalHeight / 4);
    registry.assign<ExpiringContent>(bottomLabel);
}


static void handleRateDialog(Registry &registry) {
    constexpr auto numMatchesThreshold = 2U;
    auto& settings = Locator::Settings::ref();

    if(settings.read(settingsRatingState, ratingLater) == ratingLater) {
        if(!registry.has<PlayedMatches>()) {
            auto entity = registry.create();
            registry.assign<PlayedMatches>(entt::tag_t{}, entity);
        }

        if(++registry.get<PlayedMatches>().count == numMatchesThreshold) {
            showRateDialog();
        }
    }
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
    // this way we support both solo games and multiplayer matches
    registry.destroy(registry.attachee<PlayerScore>());
}


static void clearMultiplayerGame(Registry &registry) {
    registry.remove<LetsPlay>();
    registry.remove<Timer>();
}


static void clearTraining(Registry &registry) {
    if(registry.has<LetsTrain>()) {
        registry.destroy(registry.attachee<LetsTrain>());
    }
}


static void initGame(Registry &registry, bool multiplayer = false) {
    auto game = registry.create();
    registry.assign<LetsPlay>(entt::tag_t{}, game, multiplayer);
    registry.assign<PlayerScore>(entt::tag_t{}, game);
    registry.assign<Timer>(entt::tag_t{}, game, gameDuration);
}


static void initTraining(Registry &registry) {
    auto game = registry.create();
    registry.assign<LetsTrain>(entt::tag_t{}, game);
    registry.assign<PlayerScore>(entt::tag_t{}, game);
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
        case PanelType::MULTIPLAYER_RESULTS:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::INVITE_TRAIN_LEFT:
        case PanelType::INVITE_SHARE_LEFT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::INVITE_TRAIN_RIGHT:
        case PanelType::INVITE_SHARE_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::THE_GAME_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
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

    if(type == PanelType::WALLPAPER) {
        registry.view<Panel, Transform>().each([&registry, offset](auto entity, const auto &panel, const auto &transform) {
            switch(panel.type) {
            case PanelType::BACKGROUND_TOP:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -offset, duration, 0_ui32, &easeOutElastic);
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
                break;
            case PanelType::BACKGROUND_BOTTOM:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h + offset, duration, 0_ui32, &easeOutElastic);
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
                break;
            case PanelType::GAME_OVER:
            case PanelType::INVITE_TRAIN_RIGHT:
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration / 5, 0_ui32, &easeOutCubic);
                break;
            case PanelType::INVITE_TRAIN_LEFT:
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration / 5, 0_ui32, &easeOutCubic);
                break;
            case PanelType::INVITE_SHARE_TOP:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutElastic);
                break;
            case PanelType::INVITE_SHARE_BOTTOM:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutElastic);
                break;
            default:
                // all the other panels are already out of scene (they ought to be at least)
                break;
            }
        });
    } else {
        registry.view<Panel, Transform>().each([&registry, offset](auto entity, const auto &panel, const auto &transform) {
            switch(panel.type) {
            case PanelType::MENU_TOP:
            case PanelType::BACKGROUND_TOP:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -offset, duration, 0_ui32, &easeOutElastic);
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
                break;
            case PanelType::MENU_BOTTOM:
            case PanelType::BACKGROUND_BOTTOM:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h + offset, duration, 0_ui32, &easeOutElastic);
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
                break;
            case PanelType::GAME_OVER:
            case PanelType::MULTIPLAYER_RESULTS:
            case PanelType::INVITE_TRAIN_RIGHT:
            case PanelType::INVITE_SHARE_RIGHT:
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
                break;
            case PanelType::INVITE_TRAIN_LEFT:
            case PanelType::INVITE_SHARE_LEFT:
                registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
                break;
            case PanelType::THE_GAME_TOP:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
                break;
            case PanelType::THE_GAME_BOTTOM:
            case PanelType::TRAINING_BOTTOM:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeInCubic);
                break;
            case PanelType::TRAINING_TOP:
                registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeOutCubic);
                break;
            default:
                // all the other panels are already out of scene (they ought to be at least)
                break;
            }
        });
    }

    return duration;
}


static delta_type gameAndMultiplayerTutorialTransition(Registry &registry) {
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
        case PanelType::MULTIPLAYER_RESULTS:
        case PanelType::INVITE_TRAIN_RIGHT:
        case PanelType::INVITE_SHARE_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutExpo);
            break;
        case PanelType::INVITE_TRAIN_LEFT:
        case PanelType::INVITE_SHARE_LEFT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutExpo);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type videoRecordingStartTransition(Registry &) {
    static constexpr delta_type duration = 3000_ui32;
    return duration;
}


static delta_type videoRecordingStopTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::INVITE_TRAIN_LEFT:
        case PanelType::INVITE_TRAIN_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeOutElastic);
            break;
        case PanelType::INVITE_SHARE_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration / 5, 0_ui32, &easeOutCubic);
            break;
        case PanelType::INVITE_SHARE_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration / 5, 0_ui32, &easeOutCubic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type multiplayerRecordingStopTransition(Registry &registry) {
     static constexpr delta_type duration = 1000_ui32;

     registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
         switch(panel.type) {
         case PanelType::THE_GAME_BOTTOM:
             registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight, duration, 0_ui32, &easeOutExpo);
             break;
         case PanelType::INVITE_TRAIN_LEFT:
         case PanelType::INVITE_TRAIN_RIGHT:
             registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeOutElastic);
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


static delta_type multiplayerTransition(Registry &registry) {
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
        case PanelType::INVITE_SHARE_LEFT:
        case PanelType::INVITE_SHARE_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeOutElastic);
            break;
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    return duration;
}


static delta_type multiplayerShareTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::INVITE_SHARE_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::INVITE_SHARE_LEFT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::THE_GAME_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeInCubic);
            break;
        default:
            // all the other panels are already in the right place (they ought to be at least)
            break;
        }
    });

    // used to suppress wrong warnings from g++
    (void)duration;

    return {};
}


static delta_type gameOverTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;
    static constexpr delta_type transition = 3200_ui32;

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
        default:
            // all the other panels are already out of scene (they ought to be at least)
            break;
        }
    });

    // used to suppress wrong warnings from g++
    (void)duration;

    return transition;
}


static delta_type multiplayerResultsTransition(Registry &registry) {
    static constexpr delta_type duration = 1000_ui32;
    static constexpr delta_type transition = 3200_ui32;

    registry.view<Panel, Transform>().each([&registry](auto entity, const auto &panel, const auto &transform) {
        switch(panel.type) {
        case PanelType::BACKGROUND_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), 0, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::BACKGROUND_BOTTOM:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), logicalHeight - panel.h, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::MULTIPLAYER_RESULTS:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), 0, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::THE_GAME_TOP:
            registry.accommodate<VerticalAnimation>(entity, static_cast<int>(transform.y), -panel.h, duration, 0_ui32, &easeInCubic);
            break;
        case PanelType::INVITE_SHARE_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutCubic);
            break;
        case PanelType::INVITE_SHARE_LEFT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutCubic);
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
        case PanelType::GAME_OVER:
        case PanelType::MULTIPLAYER_RESULTS:
        case PanelType::INVITE_TRAIN_RIGHT:
        case PanelType::INVITE_SHARE_RIGHT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), logicalWidth, duration, 0_ui32, &easeOutExpo);
            break;
        case PanelType::INVITE_TRAIN_LEFT:
        case PanelType::INVITE_SHARE_LEFT:
            registry.accommodate<HorizontalAnimation>(entity, static_cast<int>(transform.x), -panel.w, duration, 0_ui32, &easeOutExpo);
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
    // events are filtered out during transitions
    if(curr == next) {
        const auto nextRequiresCamera = event.scene == SceneType::GAME_TUTORIAL
                || event.scene == SceneType::TRAINING_TUTORIAL
                || event.scene == SceneType::MULTIPLAYER_TUTORIAL;

        // forces camera permission page if required (game/training not allowed)
        if(nextRequiresCamera && Locator::Permissions::ref().status(PermissionType::CAMERA) != PermissionStatus::GRANTED) {
            pending = event.scene;
            Locator::Permissions::ref().request(PermissionType::CAMERA);
        } else {
            next = event.scene;
        }
    }
}


void SceneSystem::receive(const KeyboardEvent &event) noexcept {
    // inputs from keyboard are filtered out during transitions
    if(curr == next && event.type == KeyboardEvent::Type::BACK) {
        auto &dispatcher = Locator::Dispatcher::ref();

        switch(curr) {
        case SceneType::INFO_PAGE:
        case SceneType::MULTIPLAYER_PAGE:
        case SceneType::SETTINGS_PAGE:
        case SceneType::TRAINING_SELECT:
        case SceneType::GAME_RECORDING_STOP:
        case SceneType::GAME_OVER_NO_REFRESH:
        case SceneType::MULTIPLAYER_RECORDING_STOP:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
            break;
        case SceneType::WALLPAPER_PAGE:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_OVER_NO_REFRESH);
            break;
        case SceneType::TRAINING:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING_IS_OVER);
            break;
        case SceneType::MENU_PAGE:
            dispatcher.enqueue<SceneChangeEvent>(SceneType::EXIT);
            break;
        case SceneType::EXIT:
            dispatcher.enqueue<QuitEvent>();
            break;
        case SceneType::THE_GAME:
        case SceneType::MULTIPLAYER:
        case SceneType::MULTIPLAYER_SHARE:
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
        Locator::GameServices::ref().multiplayer().terminateMatch();
        next = (curr == next) ? pending : next;
        pending = SceneType::UNKNOWN;
    }
}


void SceneSystem::update(Registry &registry, delta_type delta) {
    auto &gameServices = Locator::GameServices::ref();
    auto &dispatcher = Locator::Dispatcher::ref();

    if(curr != next) {
        auto &avRecorder = Locator::AvRecorder::ref();
        auto &camera = Locator::Camera::ref();
        auto &ads = Locator::Ads::ref();
        auto &billing = Locator::Billing::ref();

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
                    break;
                case SceneType::INFO_PAGE:
                    enableUIButtons(registry, PanelType::BACKGROUND_BOTTOM);
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableUIButtons(registry, PanelType::INFO);
                    break;
                case SceneType::WALLPAPER_PAGE:
                    enableUIButtons(registry, PanelType::BACKGROUND_BOTTOM);
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableUIButtons(registry, PanelType::WALLPAPER);
                    enableUIButtons(registry, PanelType::INVITE_SHARE_TOP);
                    enableUIButtons(registry, PanelType::INVITE_SHARE_BOTTOM);
                    break;
                case SceneType::MULTIPLAYER_PAGE:
                    enableUIButtons(registry, PanelType::BACKGROUND_BOTTOM);
                    enableUIButtons(registry, PanelType::BACKGROUND_TOP);
                    enableUIButtons(registry, PanelType::MULTIPLAYER);
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
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_RECORDING_START);
                    hideBackgroundPanels(registry);
                    // starting the camera freezes the app, this seems to be a good point to risk a lag
                    camera.start();
                    break;
                case SceneType::MULTIPLAYER_TUTORIAL:
                    dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_PLAY, true);
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MULTIPLAYER_RECORDING_START);
                    hideBackgroundPanels(registry);
                    // starting the camera freezes the app, this seems to be a good point to risk a lag
                    camera.start();
                    break;
                case SceneType::GAME_RECORDING_START:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::THE_GAME);
                    break;
                case SceneType::MULTIPLAYER_RECORDING_START:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MULTIPLAYER);
                    break;
                case SceneType::GAME_RECORDING_STOP:
                    enableUIButtons(registry, PanelType::GAME_OVER);
                    enableUIButtons(registry, PanelType::INVITE_TRAIN_RIGHT);
                    handleRateDialog(registry);
                    [&dispatcher](bool openShare) {
                        if(openShare) {
                            dispatcher.enqueue<SceneChangeEvent>(SceneType::WALLPAPER_PAGE);
                        }
                    }(avRecorder.supportExport());
                    break;
                case SceneType::MULTIPLAYER_RECORDING_STOP:
                    enableUIButtons(registry, PanelType::MULTIPLAYER_RESULTS);
                    enableUIButtons(registry, PanelType::INVITE_TRAIN_RIGHT);
                    // terminates the match and therefore leaves the room
                    Locator::GameServices::ref().multiplayer().terminateMatch();
                    handleRateDialog(registry);
                    break;
                case SceneType::THE_GAME:
                    enableCameraFrame(registry);
                    enableStanza(registry, StanzaType::IN_GAME_ITEM_TUTORIAL);
                    ads.load(AdsType::INTERSTITIAL);
                    initGame(registry);
                    break;
                case SceneType::MULTIPLAYER:
                    enableCameraFrame(registry);
                    enableUIButtons(registry, PanelType::INVITE_SHARE_RIGHT);
                    ads.load(AdsType::INTERSTITIAL);
                    initGame(registry, true);
                    break;
                case SceneType::MULTIPLAYER_SHARE:
                    enableCameraFrame(registry);
                    break;
                case SceneType::GAME_OVER:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::GAME_RECORDING_STOP);
                    break;
                case SceneType::GAME_OVER_NO_REFRESH:
                    enableUIButtons(registry, PanelType::GAME_OVER);
                    enableUIButtons(registry, PanelType::INVITE_TRAIN_RIGHT);
                    break;
                case SceneType::MULTIPLAYER_RESULTS:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::MULTIPLAYER_RECORDING_STOP);
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
                    ads.load(AdsType::INTERSTITIAL);
                    initTraining(registry);
                    break;
                case SceneType::TRAINING_IS_OVER:
                    dispatcher.enqueue<SceneChangeEvent>(SceneType::TRAINING_SELECT);
                    enableCameraFrame(registry);
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
            disableStanza(registry);
            disableFaceButtons(registry);
            disableUIButtons(registry);
            hideFaceButtons(registry);
            hideSmashButtons(registry);
            hidePopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
            hidePopupButtons(registry, PanelType::BACKGROUND_TOP);
            hidePopupButtons(registry, PanelType::INFO);
            hidePopupButtons(registry, PanelType::MULTIPLAYER);
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
            case SceneType::INFO_PAGE:
                discardExpiringContents(registry);
                refreshInfoPanel(registry);
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::INFO);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::INFO);
                break;
            case SceneType::WALLPAPER_PAGE:
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::WALLPAPER);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::WALLPAPER);
                break;
            case SceneType::MULTIPLAYER_PAGE:
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::MULTIPLAYER);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::MULTIPLAYER);
                break;
            case SceneType::SETTINGS_PAGE:
                showPopupButtons(registry, PanelType::BACKGROUND_BOTTOM);
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showPopupButtons(registry, PanelType::SETTINGS);
                hideBackgroundPanels(registry);
                remaining = bgPanelTransition(registry, PanelType::SETTINGS);
                break;
            case SceneType::MENU_PAGE:
                gameServices.multiplayer().fetchInvitations();
                dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_RELAX, false);
                Locator::GameServices::ref().multiplayer().terminateMatch();
                remaining = menuPageTransition(registry);
                break;
            case SceneType::GAME_TUTORIAL:
            case SceneType::MULTIPLAYER_TUTORIAL:
                dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_PLAY, false);
                dispatcher.enqueue<ArmageddonEvent>();
                discardExpiringContents(registry);
                remaining = gameAndMultiplayerTutorialTransition(registry);
                break;
            case SceneType::GAME_RECORDING_START:
                // video recording has a bootstrap time we want to manage to create better videos
                avRecorder.start(recordingWidth, recordingHeight);
                // read carefully the rules of the game!
                remaining = videoRecordingStartTransition(registry);
                break;
            case SceneType::MULTIPLAYER_RECORDING_START:
#ifdef DEBUG
                // video recording has a bootstrap time we want to manage to create better videos
                avRecorder.start(recordingWidth, recordingHeight);
#endif // DEBUG
                // read carefully the rules of the game!
                remaining = videoRecordingStartTransition(registry);
                break;
            case SceneType::GAME_RECORDING_STOP:
                avRecorder.stop();
                showPopupButtons(registry, PanelType::GAME_OVER);
                ads.isLoaded(AdsType::INTERSTITIAL) ? ads.show(AdsType::INTERSTITIAL) : void();
                // we try to create enough room to finalize the video
                remaining = videoRecordingStopTransition(registry);
                break;
            case SceneType::MULTIPLAYER_RECORDING_STOP:
#ifdef DEBUG
                avRecorder.stop();
#endif // DEBUG
                refreshResultsMultiplayerResultsPanel(registry);
                clearGame(registry);
                showPopupButtons(registry, PanelType::MULTIPLAYER_RESULTS);
                ads.isLoaded(AdsType::INTERSTITIAL) ? ads.show(AdsType::INTERSTITIAL) : void();
                // we try to create enough room to finalize the video
                remaining = multiplayerRecordingStopTransition(registry);
                break;
            case SceneType::THE_GAME:
                showSmashButtons(registry);
                enableCameraFrame(registry);
                remaining = theGameTransition(registry);
                break;
            case SceneType::MULTIPLAYER:
                showSmashButtons(registry);
                enableCameraFrame(registry);
                remaining = multiplayerTransition(registry);
                break;
            case SceneType::MULTIPLAYER_SHARE:
                showSmashButtons(registry);
                enableCameraFrame(registry);
                remaining = multiplayerShareTransition(registry);
                break;
            case SceneType::GAME_OVER:
                dispatcher.enqueue<ArmageddonEvent>();
                discardExpiringContents(registry);
                refreshGameOverPanel(registry);
                clearGame(registry);
                showShareMessages(registry);
                resetPulseButton(registry);
                hidePopupButtons(registry, PanelType::GAME_OVER);
                remaining = gameOverTransition(registry);
                break;
            case SceneType::GAME_OVER_NO_REFRESH:
                // fake transitions, we reuse (and abuse) existing functions so
                // as not to rewrite the same code again and again ... :+1: :-)
                gameOverTransition(registry);
                remaining = videoRecordingStopTransition(registry);
                break;
            case SceneType::MULTIPLAYER_RESULTS:
                dispatcher.enqueue<ArmageddonEvent>();
                discardExpiringContents(registry);
                refreshButtonsMultiplayerResultsPanel(registry);
                clearMultiplayerGame(registry);
                showShareMessages(registry);
                resetPulseButton(registry);
                hidePopupButtons(registry, PanelType::MULTIPLAYER_RESULTS);
                remaining = multiplayerResultsTransition(registry);
                break;
            case SceneType::TRAINING_TUTORIAL:
                dispatcher.enqueue<AudioMusicEvent>(AudioMusicType::AUDIO_MUSIC_PLAY, false);
                dispatcher.enqueue<ArmageddonEvent>();
                camera.start();
                remaining = trainingTutorialTransition(registry);
                break;
            case SceneType::TRAINING_SELECT:
                showPopupButtons(registry, PanelType::BACKGROUND_TOP);
                showSmashButtons(registry);
                showFaceButtons(registry);
                enableCameraFrame(registry);
                remaining = trainingSelectTransition(registry);
                break;
            case SceneType::TRAINING:
                showSmashButtons(registry);
                remaining = trainingTransition(registry);
                break;
            case SceneType::TRAINING_IS_OVER:
                ads.isLoaded(AdsType::INTERSTITIAL) ? ads.show(AdsType::INTERSTITIAL) : void();
                clearTraining(registry);
                remaining = {};
                break;
            default:
                assert(false);
                break;
            }

            // make faces and items on screen (if any) no longer smashable
            registry.reset<Destroyable>();

            isTransitioning = true;
        }
    } else if((curr == SceneType::MENU_PAGE || curr == SceneType::MULTIPLAYER_PAGE)) {
        if(gameServices.multiplayer().readyPlayerOne() && pending != SceneType::MULTIPLAYER_TUTORIAL) {
            // in case there is a pending request for a multiplayer match, let's start the game immediately
            dispatcher.enqueue<SceneChangeEvent>(SceneType::MULTIPLAYER_TUTORIAL);
        } else if(!gameServices.multiplayer().readyPlayerOne()) {
            // someone invited me to a multiplayer match in the meantime? let's see...
            gameServices.multiplayer().handleInvitations();
        }
    }

}


}
