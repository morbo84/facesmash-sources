#include <type_traits>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "game_factory.h"


namespace gamee {


void createSplashScreen(Registry &registry) {
    auto panel = registry.create();
    registry.assign<Panel>(panel, logicalWidth, logicalHeight, PanelType::SPLASH_SCREEN);
    registry.assign<Transform>(panel, panel, -1.f * logicalWidth, 0.f);

    auto gamee = Locator::TextureCache::ref().handle("str/gamee");
    auto company = registry.create();
    registry.assign<Renderable>(company, 0.f, 20);
    registry.assign<Sprite>(company, gamee, gamee->width(), gamee->height(), gamee->width(), gamee->height());
    registry.assign<Transform>(company, panel, logicalWidth / 2.f - gamee->width() / 2.f, logicalHeight / 2.f - gamee->height() / 2.f);
    registry.assign<FadeAnimation>(company, 0, 255, 2000_ui32);
}


void createBackgroundTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto topHandle = textureCache.handle("palette/bg_top");

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, -1.f * (logicalHeight - topHandle->height()));
    registry.assign<Panel>(panel, logicalWidth, logicalHeight - topHandle->height(), PanelType::BACKGROUND_TOP_PANEL);

    for(auto idx = 0, cnt = (logicalHeight / topHandle->height()) - 2; idx < cnt; ++idx) {
        auto top = registry.create();
        registry.assign<Renderable>(top);
        registry.assign<Sprite>(top, topHandle, topHandle->width(), topHandle->height(), topHandle->width(), topHandle->height());
        registry.assign<Transform>(top, panel, 0.f, 1.f * topHandle->height() * idx);
    }

    auto middle = registry.create();
    auto middleHandle = textureCache.handle("palette/bg_middle");
    registry.assign<Renderable>(middle);
    registry.assign<Sprite>(middle, middleHandle, middleHandle->width(), middleHandle->height(), middleHandle->width(), middleHandle->height());
    registry.assign<Transform>(middle, panel, 0.f, 1.f * logicalHeight - 2 * middleHandle->height());
}


void createBackgroundBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto bottomHandle = textureCache.handle("palette/bg_bottom");

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, bottomHandle->height(), PanelType::BACKGROUND_BOTTOM_PANEL);

    auto bottom = registry.create();
    registry.assign<Renderable>(bottom);
    registry.assign<Sprite>(bottom, bottomHandle, bottomHandle->width(), bottomHandle->height(), bottomHandle->width(), bottomHandle->height());
    registry.assign<Transform>(bottom, panel, 0.f, 0.f);
}


void createMenuTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto &audioService = Locator::Audio::ref();

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, -logicalHeight / 2.f);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::MENU_TOP_PANEL);

    // textureCache.load<SDLTextureLoader>("palette/separator", "png/palette/mario/separator.png", renderer);

    SDLTextureHandle facesmashHandle[9] = {
        textureCache.handle("str/facesmash/red/F"),
        textureCache.handle("str/facesmash/lightblue/A"),
        textureCache.handle("str/facesmash/yellow/C"),
        textureCache.handle("str/facesmash/blue/E"),
        textureCache.handle("str/facesmash/green/S"),
        textureCache.handle("str/facesmash/lightblue/M"),
        textureCache.handle("str/facesmash/yellow/A"),
        textureCache.handle("str/facesmash/red/S"),
        textureCache.handle("str/facesmash/green/H")
    };

    auto x = logicalWidth / 2.f - 2.f * facesmashHandle[0]->width();

    for(auto i = 0; i < 4; ++i) {
        auto entity = registry.create();
        auto handle = facesmashHandle[i];
        registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<Renderable>(entity, 0.f, 150);
        registry.assign<Transform>(entity, panel, x, .2f * handle->height());
        x += handle->width();
    }

    x = logicalWidth / 2.f - 2.5f * facesmashHandle[0]->width();

    for(auto i = 4; i < 9; ++i) {
        auto entity = registry.create();
        auto handle = facesmashHandle[i];
        registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<Renderable>(entity, 0.f, 150);
        registry.assign<Transform>(entity, panel, x, 1.4f * handle->height());
        x += handle->width();
    }

    auto separator = registry.create();
    auto separatorHandle = textureCache.handle("palette/separator");
    registry.assign<Sprite>(separator, separatorHandle, separatorHandle->width(), separatorHandle->height(), separatorHandle->width(), separatorHandle->height());
    registry.assign<Renderable>(separator, 0.f, 150);
    registry.assign<Transform>(separator, panel, logicalWidth /2.f - separatorHandle->width() / 2.f, 2.6f * facesmashHandle[0]->height());

    auto audioButton = registry.create();
    auto audioButtonHandle = audioService.isMute() ? textureCache.handle("button/mute") : textureCache.handle("button/sound");
    registry.assign<Sprite>(audioButton, audioButtonHandle, audioButtonHandle->width(), audioButtonHandle->height(), audioButtonHandle->width(), audioButtonHandle->height());
    registry.assign<Renderable>(audioButton, 0.f, 150);
    registry.assign<Transform>(audioButton, panel, logicalWidth - 3.f * audioButtonHandle->width() / 2.f, 350.f + audioButtonHandle->height() / 2.f);
    registry.assign<UIButton>(audioButton, UIAction::SWITCH_AUDIO);

    auto creditsButton = registry.create();
    auto creditsButtonHandle = textureCache.handle("button/credits");
    registry.assign<Sprite>(creditsButton, creditsButtonHandle, creditsButtonHandle->width(), creditsButtonHandle->height(), creditsButtonHandle->width(), creditsButtonHandle->height());
    registry.assign<Renderable>(creditsButton, 0.f, 150);
    registry.assign<Transform>(creditsButton, panel, creditsButtonHandle->width() / 2.f, 350.f + creditsButtonHandle->height() / 2.f);
    registry.assign<UIButton>(creditsButton, UIAction::CREDITS);

    auto theGameButton = registry.create();
    auto theGameHandle = textureCache.handle("face/happy");

    registry.assign<Renderable>(theGameButton, 0.f, 160);
    registry.assign<Transform>(theGameButton, panel, logicalWidth / 4.f - theGameHandle->width() / 2.f, 350.f + logicalHeight / 4.f - theGameHandle->height() / 2.f);
    registry.assign<Sprite>(theGameButton, theGameHandle, theGameHandle->width(), theGameHandle->height(), theGameHandle->width(), theGameHandle->height());
    registry.assign<UIButton>(theGameButton, UIAction::GAME_TUTORIAL);

    auto trainingButton = registry.create();
    auto trainingHandle = textureCache.handle("face/surprised");

    registry.assign<Renderable>(trainingButton, 0.f, 160);
    registry.assign<Transform>(trainingButton, panel, 3.f * logicalWidth / 4.f - theGameHandle->width() / 2.f, 350.f + logicalHeight / 4.f - theGameHandle->height() / 2.f);
    registry.assign<Sprite>(trainingButton, trainingHandle, trainingHandle->width(), trainingHandle->height(), trainingHandle->width(), trainingHandle->height());
    registry.assign<UIButton>(trainingButton, UIAction::TRAINING_TUTORIAL);
}


void createMenuBottomPanel(Registry &registry) {
    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::MENU_BOTTOM_PANEL);
}


void createCreditsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, -720.f, logicalHeight / 2.f - 475.f);
    registry.assign<Panel>(panel, 720, 950, PanelType::CREDITS_PANEL);

    auto window = registry.create();
    auto windowHandle = textureCache.handle("gui/window");
    registry.assign<Renderable>(window, 0.f, 210);
    registry.assign<Sprite>(window, windowHandle, windowHandle->width(), windowHandle->height(), windowHandle->width(), windowHandle->height());
    registry.assign<Transform>(window, panel, 0.f, 0.f);

    auto menu = registry.create();
    auto menuHandle = textureCache.handle("button/menu");
    registry.assign<Renderable>(menu, 0.f, 220);
    registry.assign<Sprite>(menu, menuHandle, menuHandle->width(), menuHandle->height(), menuHandle->width(), menuHandle->height());
    registry.assign<Transform>(menu, panel, windowHandle->width() / 2.f - menuHandle->width() / 2.f, windowHandle->height() - 2.f * menuHandle->height() / 3.f);
    registry.assign<UIButton>(menu, UIAction::MENU);
}


void createTutorialTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto refHandle = textureCache.handle("button/empty");

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, -logicalHeight / 2.f);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::TUTORIAL_TOP_PANEL);

    auto addButton = [panel, &textureCache, &registry](TextureCache::resource_type face, int idx) {
        auto button = registry.create();
        auto buttonHandle = textureCache.handle("button/empty");
        const auto buttonOffset = (logicalWidth - (numberOfFaces * buttonHandle->width() + (numberOfFaces - 1) * 10)) / 2;

        registry.assign<Renderable>(button, 0.f, 150, 180);
        registry.assign<Sprite>(button, buttonHandle, buttonHandle->width(), buttonHandle->height(), buttonHandle->width(), buttonHandle->height());
        registry.assign<Transform>(button, panel, buttonOffset + idx * (buttonHandle->width() + 10.f), logicalHeight / 2.f - 3.f * buttonHandle->height() / 2.f);

        auto emoji = registry.create();
        auto emojiHandle = textureCache.handle(face);

        registry.assign<Renderable>(emoji, 0.f, 160);
        registry.assign<Sprite>(emoji, emojiHandle, emojiHandle->width(), emojiHandle->height(), 3 * buttonHandle->width() / 5, 3 * buttonHandle->height() / 5);
        registry.assign<Transform>(emoji, button, buttonHandle->width() / 5.f, buttonHandle->height() / 5.f);
    };

    addButton("face/angry", 0);
    addButton("face/disgusted", 1);
    addButton("face/happy", 2);
    addButton("face/surprised", 3);
    addButton("face/fearful", 4);
    addButton("face/sad", 5);

    auto entity = registry.create();
    auto handle = textureCache.handle("str/tutorial/face");
    registry.assign<Renderable>(entity, 0.f, 150);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, panel, logicalWidth / 2.f - handle->width() / 2.f, logicalHeight / 2.f - 3.f * refHandle->height() / 2.f - handle->height() - 10.f);
}


void createTutorialBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto refHandle = textureCache.handle("button/empty");

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::TUTORIAL_BOTTOM_PANEL);

    auto addButton = [panel, &textureCache, &registry](TextureCache::resource_type face, int idx) {
        auto button = registry.create();
        auto buttonHandle = textureCache.handle("button/empty");
        const auto buttonOffset = (logicalWidth - (numberOfItems * buttonHandle->width() + (numberOfItems - 1) * 10)) / 2;

        registry.assign<Renderable>(button, 0.f, 150, 180);
        registry.assign<Sprite>(button, buttonHandle, buttonHandle->width(), buttonHandle->height(), buttonHandle->width(), buttonHandle->height());
        registry.assign<Transform>(button, panel, buttonOffset + idx * (buttonHandle->width() + 10.f), buttonHandle->height() / 2.f);

        auto emoji = registry.create();
        auto emojiHandle = textureCache.handle(face);

        registry.assign<Renderable>(emoji, 0.f, 160);
        registry.assign<Sprite>(emoji, emojiHandle, emojiHandle->width(), emojiHandle->height(), 3 * buttonHandle->width() / 5, 3 * buttonHandle->height() / 5);
        registry.assign<Transform>(emoji, button, buttonHandle->width() / 5.f, buttonHandle->height() / 5.f);
    };

    addButton("item/speed_up", 0);
    addButton("item/slow_down", 1);
    addButton("item/fountain", 2);

    auto entity = registry.create();
    auto handle = textureCache.handle("str/tutorial/touch");
    registry.assign<Renderable>(entity, 0.f, 150);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, panel, logicalWidth / 2.f - handle->width() / 2.f, 3.f * refHandle->height() / 2.f + 10.f);
}


void createGameTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto sym0Handle = toStrHud(0);
    auto symEmptyHandle = textureCache.handle("str/hud/ ");
    auto scoreHandle = textureCache.handle("str/hud/score");
    auto timerHandle = textureCache.handle("str/hud/timer");

    float offset = 0.f;

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, -logicalHeight / 2.f);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::GAME_TOP_PANEL);

    auto playerScoreEntity = registry.create();
    registry.assign<Renderable>(playerScoreEntity, 0.f, 160);
    auto &playerScore = registry.attach<PlayerScore>(playerScoreEntity);
    registry.accomodate<HUD>(playerScoreEntity, scoreHandle, scoreHandle->width(), scoreHandle->height(), scoreHandle->width(), scoreHandle->height());
    registry.accomodate<Transform>(playerScoreEntity, panel, offset + .2f * scoreHandle->width(), .4f * scoreHandle->height());
    offset = registry.get<Transform>(playerScoreEntity).x + 1.2f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(PlayerScore::entities)>::value; ++i) {
        auto entity = registry.create();
        auto handle = i ? symEmptyHandle : sym0Handle;
        playerScore.entities[i] = entity;
        registry.assign<Renderable>(entity, 0.f, 160);
        registry.assign<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<Transform>(entity, panel, offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    offset = logicalWidth - .4f * scoreHandle->width() - 2 * sym0Handle->width() - 1.f * timerHandle->width();

    auto gamerTimerEntity = registry.create();
    registry.assign<Renderable>(gamerTimerEntity, 0.f, 160);
    auto &gameTimer = registry.attach<GameTimer>(gamerTimerEntity);
    registry.accomodate<HUD>(gamerTimerEntity, timerHandle, timerHandle->width(), timerHandle->height(), timerHandle->width(), timerHandle->height());
    registry.accomodate<Transform>(gamerTimerEntity, panel, offset, .4f * scoreHandle->height());
    offset = registry.get<Transform>(gamerTimerEntity).x + timerHandle->width() + .2f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(GameTimer::entities)>::value; ++i) {
        auto entity = registry.create();
        auto handle = i ? symEmptyHandle : sym0Handle;
        gameTimer.entities[i] = entity;
        registry.assign<Renderable>(entity, 0.f, 160);
        registry.assign<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<Transform>(entity, panel, offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    auto reward = registry.create();
    auto perfectHandle = textureCache.handle("str/reward/perfect");
    registry.assign<Renderable>(reward, 0.f, 160);
    registry.assign<Transform>(reward, reward, logicalWidth / 2.f, logicalHeight / 6.f - 1.4f * perfectHandle->height());
    registry.attach<Reward>(reward);
}


void createGameBottomPanel(Registry &registry) {
    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::GAME_BOTTOM_PANEL);
}


void createGameOverPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, logicalWidth / 2.f - 450.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, 900, 1130, PanelType::GAME_OVER_PANEL);

    auto ribbon = registry.create();
    auto ribbonHandle = textureCache.handle("gui/ribbon");
    registry.assign<Renderable>(ribbon, 0.f, 220);
    registry.assign<Sprite>(ribbon, ribbonHandle, ribbonHandle->width(), ribbonHandle->height(), ribbonHandle->width(), ribbonHandle->height());
    registry.assign<Transform>(ribbon, panel, 0.f, 0.f);

    auto popup = registry.create();
    auto popupHandle = textureCache.handle("gui/popup");
    registry.assign<Renderable>(popup, 0.f, 210);
    registry.assign<Sprite>(popup, popupHandle, popupHandle->width(), popupHandle->height(), popupHandle->width(), popupHandle->height());
    const auto &popupTransform = registry.assign<Transform>(popup, panel, ribbonHandle->width() / 2.f - popupHandle->width() / 2.f, ribbonHandle->height() / 2.f);

    auto menu = registry.create();
    auto menuHandle = textureCache.handle("button/menu");
    registry.assign<Renderable>(menu, 0.f, 220);
    registry.assign<Sprite>(menu, menuHandle, menuHandle->width(), menuHandle->height(), menuHandle->width(), menuHandle->height());
    registry.assign<Transform>(menu, panel, popupTransform.x + popupHandle->width() / 4.f - menuHandle->width() / 2.f, ribbonHandle->height() / 2.f + popupHandle->height() - 2.f * menuHandle->height() / 3.f);
    registry.assign<UIButton>(menu, UIAction::MENU);

    auto share = registry.create();
    auto shareHandle = textureCache.handle("button/share");
    registry.assign<Renderable>(share, 0.f, 220);
    registry.assign<Sprite>(share, shareHandle, shareHandle->width(), shareHandle->height(), shareHandle->width(), shareHandle->height());
    registry.assign<Transform>(share, panel, popupTransform.x + popupHandle->width() / 2.f - shareHandle->width() / 2.f, popupTransform.y + popupHandle->height() - 2.f * shareHandle->height() / 3.f);
    registry.assign<UIButton>(share, UIAction::SHARE);

    auto reload = registry.create();
    auto reloadHandle = textureCache.handle("button/reload");
    registry.assign<Renderable>(reload, 0.f, 220);
    registry.assign<Sprite>(reload, reloadHandle, reloadHandle->width(), reloadHandle->height(), reloadHandle->width(), reloadHandle->height());
    registry.assign<Transform>(reload, panel, popupTransform.x + 3.f * popupHandle->width() / 4.f - reloadHandle->width() / 2.f, popupTransform.y + popupHandle->height() - 2.f * reloadHandle->height() / 3.f);
    registry.assign<UIButton>(reload, UIAction::RELOAD);
}


void createTrainingTopPanel(Registry &registry) {
    // TODO
}


void createTrainingBottomPanel(Registry &registry) {
    // TODO
}


void createCamera(Registry &registry) {
    auto camera = registry.create();
    registry.assign<Transform>(camera, camera, 0.f, 0.f);
    registry.attach<Camera>(camera);
}


#ifdef CAMERA_FRAME_AVAILABLE
void createCameraFrame(Registry &registry) {
    auto handle = Locator::TextureCache::ref().handle("camera/frame");
    auto &cameraService = Locator::Camera::ref();

    auto frame = registry.create();
    registry.attach<CameraFrame>(frame);

    if(cameraService.height() > cameraService.width()) {
        registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), logicalWidth, handle->height() * logicalWidth / handle->width());
        registry.assign<Transform>(frame, frame, 0.f, (logicalHeight - handle->height()) / 2.f);
        registry.assign<Renderable>(frame, 0.f, 90, 0);
    } else {
        const int width = handle->width() * logicalWidth / handle->height();
        const int height = logicalWidth;

        registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), width, height);
        registry.assign<Transform>(frame, frame, (height - width) / 2.f, (width - height) / 2.f);
        registry.assign<Renderable>(frame, -90.f, 90, 0);
    }
}
#endif // CAMERA_FRAME_AVAILABLE


#ifdef DEBUG
void createSmashButtons(Registry &registry) {
    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, logicalWidth + 96.f, logicalHeight / 2.f - 368.f);
    registry.assign<Panel>(panel, 96, 736, PanelType::SMASH_BUTTONS_PANEL);

    auto addButton = [panel, &registry](FaceType type, int idx) {
        auto button = registry.create();
        auto handle = toHandle(type);

        registry.assign<SmashButton>(button, type);
        registry.assign<Transform>(button, panel, 0.f, idx * (96.f + 32.f));
        registry.assign<Sprite>(button, handle, handle->width(), handle->height(), 96, 96);
        registry.assign<Renderable>(button, 0.f, 220);
        registry.assign<BoundingBox>(button, 96, 96);
    };

    int pos = 0;

    addButton(FaceType::ANGRY, pos++);
    addButton(FaceType::DISGUSTED, pos++);
    addButton(FaceType::HAPPY, pos++);
    addButton(FaceType::SURPRISED, pos++);
    addButton(FaceType::FEARFUL, pos++);
    addButton(FaceType::SAD, pos++);
}


void createDebugHUD(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto sym0Handle = toStrDebug(0);
    float offset = 0.f;

    auto fpsDebugEntity = registry.create();
    auto fpsHandle = textureCache.handle("str/debug/fps");
    registry.assign<Renderable>(fpsDebugEntity, 0.f, 255);
    auto &fpsDebug = registry.attach<FPSDebug>(fpsDebugEntity);
    registry.accomodate<HUD>(fpsDebugEntity, fpsHandle, fpsHandle->width(), fpsHandle->height(), fpsHandle->width(), fpsHandle->height());
    registry.accomodate<Transform>(fpsDebugEntity, fpsDebugEntity, offset + .2f * fpsHandle->width(), logicalHeight - 1.2f * fpsHandle->height());
    offset = registry.get<Transform>(fpsDebugEntity).x + 1.2f * fpsHandle->width();

    for(auto i = 0u; i < std::extent<decltype(FPSDebug::entities)>::value; ++i) {
        auto entity = registry.create();
        fpsDebug.entities[i] = entity;
        registry.assign<Renderable>(entity, 0.f, 255);
        registry.assign<HUD>(entity, sym0Handle, sym0Handle->width(), sym0Handle->height(), sym0Handle->width(), sym0Handle->height());
        registry.assign<Transform>(entity, entity, offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
        offset += sym0Handle->width();
    }

    auto timeDebugEntity = registry.create();
    auto timeHandle = textureCache.handle("str/debug/time");
    registry.assign<Renderable>(timeDebugEntity, 0.f, 255);
    auto &timeDebug = registry.attach<TimeDebug>(timeDebugEntity);
    registry.accomodate<HUD>(timeDebugEntity, timeHandle, timeHandle->width(), timeHandle->height(), timeHandle->width(), timeHandle->height());
    registry.accomodate<Transform>(timeDebugEntity, timeDebugEntity, offset + .5f * timeHandle->width(), logicalHeight - .2f * fpsHandle->height() - timeHandle->height());
    offset = registry.get<Transform>(timeDebugEntity).x + 1.2f * timeHandle->width();

    auto timeDebugFirstEntity = registry.create();
    timeDebug.entities[0] = timeDebugFirstEntity;
    registry.assign<Renderable>(timeDebugFirstEntity, 0.f, 255);
    registry.assign<HUD>(timeDebugFirstEntity, sym0Handle, sym0Handle->width(), sym0Handle->height(), sym0Handle->width(), sym0Handle->height());
    registry.assign<Transform>(timeDebugFirstEntity, timeDebugFirstEntity, offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
    offset += sym0Handle->width();

    auto timeDebugSecondEntity = registry.create();
    timeDebug.entities[1] = timeDebugSecondEntity;
    registry.assign<Renderable>(timeDebugSecondEntity, 0.f, 255);
    registry.assign<HUD>(timeDebugSecondEntity, sym0Handle, sym0Handle->width(), sym0Handle->height(), sym0Handle->width(), sym0Handle->height());
    registry.assign<Transform>(timeDebugSecondEntity, timeDebugSecondEntity, offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
    offset += sym0Handle->width();

    auto timeDebugDotEntity = registry.create();
    auto symDotHandle = textureCache.handle("str/debug/.");
    registry.assign<Renderable>(timeDebugDotEntity, 0.f, 255);
    registry.assign<HUD>(timeDebugDotEntity, symDotHandle, symDotHandle->width(), symDotHandle->height(), symDotHandle->width(), symDotHandle->height());
    registry.assign<Transform>(timeDebugDotEntity, timeDebugDotEntity, offset, logicalHeight - .2f * fpsHandle->height() - symDotHandle->height());
    offset += symDotHandle->width();

    auto timeDebugThirdEntity = registry.create();
    timeDebug.entities[2] = timeDebugThirdEntity;
    registry.assign<Renderable>(timeDebugThirdEntity, 0.f, 255);
    registry.assign<HUD>(timeDebugThirdEntity, sym0Handle, sym0Handle->width(), sym0Handle->height(), sym0Handle->width(), sym0Handle->height());
    registry.assign<Transform>(timeDebugThirdEntity, timeDebugThirdEntity, offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
    offset += sym0Handle->width();
}
#endif // DEBUG


}
