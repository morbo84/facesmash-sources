#include <type_traits>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "ui_factory.h"
#include "game_factory.h"


namespace gamee {


void createSplashScreen(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto panel = createPanel(registry, PanelType::SPLASH_SCREEN, -logicalWidth, 0, logicalWidth, logicalHeight);

    auto gameeHandle = textureCache.handle("str/gamee");
    auto gamee = createSprite(registry, panel, gameeHandle, 20);
    setPos(registry, gamee, logicalWidth / 2 - gameeHandle->width() / 2, logicalHeight / 2 - gameeHandle->height() / 2);
    registry.assign<FadeAnimation>(gamee, 0, 255, 2000_ui32);
}


void createBackgroundTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto topHandle = textureCache.handle("palette/bg_top");

    auto panel = createPanel(registry, PanelType::BACKGROUND_TOP_PANEL, 0, topHandle->height() - logicalHeight, logicalWidth, logicalHeight - topHandle->height());

    for(auto idx = 0, cnt = (logicalHeight / topHandle->height()) - 2; idx < cnt; ++idx) {
        auto top = createSprite(registry, panel, topHandle, 127);
        setPos(registry, top, 0, topHandle->height() * idx);
    }

    auto middleHandle = textureCache.handle("palette/bg_middle");
    auto middle = createSprite(registry, panel, middleHandle, 127);
    setPos(registry, middle, 0, logicalHeight - 2 * middleHandle->height());
}


void createBackgroundBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto bottomHandle = textureCache.handle("palette/bg_bottom");

    auto panel = createPanel(registry, PanelType::BACKGROUND_BOTTOM_PANEL, 0, logicalHeight, logicalWidth, bottomHandle->height());

    createSprite(registry, panel, bottomHandle, 127);
}


void createMenuTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto panel = createPanel(registry, PanelType::MENU_TOP_PANEL, 0, -logicalHeight / 2, logicalWidth, 5 * logicalHeight / 6);

    SDLTextureHandle faceHandles[4] = {
        textureCache.handle("str/facesmash/blue/E"),
        textureCache.handle("str/facesmash/yellow/C"),
        textureCache.handle("str/facesmash/lightblue/A"),
        textureCache.handle("str/facesmash/red/F")
    };

    SDLTextureHandle smashHandles[5] = {
        textureCache.handle("str/facesmash/green/H"),
        textureCache.handle("str/facesmash/red/S"),
        textureCache.handle("str/facesmash/yellow/A"),
        textureCache.handle("str/facesmash/lightblue/M"),
        textureCache.handle("str/facesmash/green/S")
    };

    auto x = logicalWidth - faceHandles[0]->width() / 4;

    for(auto i = 0u; i < std::extent<decltype(faceHandles)>::value; ++i) {
        x -= faceHandles[i]->width();
        auto entity = createSprite(registry, panel, faceHandles[i], 150);
        setPos(registry, entity, x, faceHandles[0]->height() / 2);
    }

    x = logicalWidth - smashHandles[0]->width() / 4;

    for(auto i = 0u; i < std::extent<decltype(smashHandles)>::value; ++i) {
        x -= smashHandles[i]->width();
        auto entity = createSprite(registry, panel, smashHandles[i], 150);
        setPos(registry, entity, x, 3 * smashHandles[0]->height() / 2);
    }

    auto theGameButtonHandle = textureCache.handle("face/happy");
    auto theGameButton = createUIButton(registry, panel, theGameButtonHandle, UIAction::GAME_TUTORIAL, 160);
    setPos(registry, theGameButton, logicalWidth / 4 - theGameButtonHandle->width() / 2, 350 + logicalHeight / 4 - theGameButtonHandle->height() / 2);

    auto trainingButtonHandle = textureCache.handle("face/surprised");
    auto trainingButton = createUIButton(registry, panel, trainingButtonHandle, UIAction::TRAINING_TUTORIAL, 160);
    setPos(registry, trainingButton, 3 * logicalWidth / 4 - trainingButtonHandle->width() / 2, 350 + logicalHeight / 4 - trainingButtonHandle->height() / 2);
}


void createMenuBottomPanel(Registry &registry) {
    constexpr auto buttonSize = 192;

    auto &textureCache = Locator::TextureCache::ref();

    auto panel = createPanel(registry, PanelType::MENU_BOTTOM_PANEL, 0, logicalHeight, logicalWidth, logicalHeight / 6);

    auto creditsButtonHandle = textureCache.handle("bt/empty");
    auto creditsButton = createUIButton(registry, panel, creditsButtonHandle, UIAction::CREDITS, 150);
    setPos(registry, creditsButton, logicalWidth / 5, logicalHeight / 6 - creditsButtonHandle->height() / 2);
    setSpriteSize(registry, creditsButton, buttonSize, buttonSize);

    auto exitButtonHandle = textureCache.handle("bt/empty");
    auto exitButton = createUIButton(registry, panel, exitButtonHandle, UIAction::EXIT, 150);
    setPos(registry, exitButton, 2 * logicalWidth / 5, logicalHeight / 6 - exitButtonHandle->height() / 2);
    setSpriteSize(registry, exitButton, buttonSize, buttonSize);

    auto audioButtonHandle = textureCache.handle("bt/empty");
    auto audioButton = createUIButton(registry, panel, audioButtonHandle, UIAction::SWITCH_AUDIO, 150);
    setPos(registry, audioButton, 3 * logicalWidth / 5, logicalHeight / 6 - audioButtonHandle->height() / 2);
    setSpriteSize(registry, audioButton, buttonSize, buttonSize);
}


void createCreditsPanel(Registry &registry) {
    constexpr auto buttonSize = 192;

    auto &textureCache = Locator::TextureCache::ref();

    auto panel = createPanel(registry, PanelType::CREDITS_PANEL, logicalWidth, 0, logicalWidth, logicalHeight);

    auto windowHandle = textureCache.handle("gui/window");
    createSprite(registry, panel, windowHandle, 210);

    auto menuHandle = textureCache.handle("bt/empty");
    auto menu = createUIButton(registry, panel, menuHandle, UIAction::MENU, 220);
    setPos(registry, menu, windowHandle->width() / 2 - menuHandle->width() / 2, windowHandle->height() - 2 * menuHandle->height() / 3);
    setSpriteSize(registry, menu, buttonSize, buttonSize);
}


void createTutorialTopPanel(Registry &registry) {
    constexpr auto buttonSize = 144;

    auto &textureCache = Locator::TextureCache::ref();

    auto panel = createPanel(registry, PanelType::TUTORIAL_TOP_PANEL, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);

    auto addButton = [panel, &textureCache, &registry](TextureCache::resource_type face, int idx) {
        auto buttonHandle = textureCache.handle("bt/empty");
        auto button = createSprite(registry, panel, buttonHandle, 150);
        const auto buttonOffset = (logicalWidth - (numberOfFaces * buttonSize + (numberOfFaces - 1) * 10)) / 2;
        setPos(registry, button, buttonOffset + idx * (buttonSize + 10), logicalHeight / 2 - 3 * buttonSize / 2);
        setSpriteSize(registry, button, buttonSize, buttonSize);

        auto emojiHandle = textureCache.handle(face);
        auto emoji = createSprite(registry, button, emojiHandle, 160);
        setSpriteSize(registry, emoji, 3 * buttonSize / 5, 3 * buttonSize / 5);
        setPos(registry, emoji, buttonSize / 5, buttonSize / 5);
    };

    addButton("face/angry", 0);
    addButton("face/disgusted", 1);
    addButton("face/happy", 2);
    addButton("face/surprised", 3);
    addButton("face/fearful", 4);
    addButton("face/sad", 5);

    auto tutorialHandle = textureCache.handle("str/tutorial/face");
    auto tutorial = createSprite(registry, panel, tutorialHandle, 150);
    setPos(registry, tutorial, logicalWidth / 2 - tutorialHandle->width() / 2, logicalHeight / 2 - 1.8 * buttonSize - tutorialHandle->height());
}


void createTutorialBottomPanel(Registry &registry) {
    constexpr auto buttonSize = 144;

    auto &textureCache = Locator::TextureCache::ref();

    auto panel = createPanel(registry, PanelType::TUTORIAL_BOTTOM_PANEL, 0, logicalHeight, logicalWidth, logicalHeight / 2);

    auto addButton = [panel, &textureCache, &registry](TextureCache::resource_type face, int idx) {
        auto buttonHandle = textureCache.handle("bt/empty");
        auto button = createSprite(registry, panel, buttonHandle, 150);
        const auto buttonOffset = (logicalWidth - (numberOfItems * buttonSize + (numberOfItems - 1) * 10)) / 2;
        setPos(registry, button, buttonOffset + idx * (buttonSize + 10), buttonSize / 2);
        setSpriteSize(registry, button, buttonSize, buttonSize);

        auto emojiHandle = textureCache.handle(face);
        auto emoji = createSprite(registry, button, emojiHandle, 160);
        setSpriteSize(registry, emoji, 3 * buttonSize / 5, 3 * buttonSize / 5);
        setPos(registry, emoji, buttonSize / 5, buttonSize / 5);
    };

    addButton("item/speed_up", 0);
    addButton("item/slow_down", 1);
    addButton("item/fountain", 2);

    auto tutorialHandle = textureCache.handle("str/tutorial/touch");
    auto tutorial = createSprite(registry, panel, tutorialHandle, 150);
    setPos(registry, tutorial, logicalWidth / 2 - tutorialHandle->width() / 2, 1.8f * buttonSize);
}


void createGameTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto sym0Handle = toStrHud(0);
    auto symEmptyHandle = textureCache.handle("str/hud/ ");
    auto scoreHandle = textureCache.handle("str/hud/score");
    auto timerHandle = textureCache.handle("str/hud/timer");

    auto offset = 0;

    auto panel = createPanel(registry, PanelType::GAME_TOP_PANEL, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 6);

    auto playerScoreEntity = createHUD(registry, panel, scoreHandle, 160);
    auto &playerScore = registry.attach<PlayerScore>(playerScoreEntity);
    setPos(registry, playerScoreEntity, offset + .2f * scoreHandle->width(), .4f * scoreHandle->height());;
    offset = registry.get<Transform>(playerScoreEntity).x + 1.2f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(PlayerScore::entities)>::value; ++i) {
        auto handle = i ? symEmptyHandle : sym0Handle;
        playerScore.entities[i] = createHUD(registry, panel, handle, 160);
        setPos(registry, playerScore.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    offset = logicalWidth - .4f * scoreHandle->width() - 2 * sym0Handle->width() - timerHandle->width();

    auto gameTimerEntity = createHUD(registry, panel, timerHandle, 160);
    auto &gameTimer = registry.attach<GameTimer>(gameTimerEntity);
    setPos(registry, gameTimerEntity, offset, .4f * scoreHandle->height());;
    offset = registry.get<Transform>(gameTimerEntity).x + timerHandle->width() + .2f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(GameTimer::entities)>::value; ++i) {
        auto handle = i ? symEmptyHandle : sym0Handle;
        gameTimer.entities[i] = createHUD(registry, panel, handle, 160);
        setPos(registry, gameTimer.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    auto rewardHandle = textureCache.handle("str/reward/perfect");
    auto reward = createHUD(registry, panel, rewardHandle, 160);
    setPos(registry, reward, logicalWidth / 2 - rewardHandle->width() / 2, logicalHeight / 6 - 1.4f * rewardHandle->height());
    registry.get<Renderable>(reward).alpha = 0;
    registry.attach<Reward>(reward);
}


void createGameBottomPanel(Registry &registry) {
    auto panel = createPanel(registry, PanelType::GAME_BOTTOM_PANEL, 0, logicalHeight, logicalWidth, logicalHeight / 6);
    // TODO
    (void)panel;
}


void createGameOverPanel(Registry &registry) {
    constexpr auto buttonSize = 192;

    auto &textureCache = Locator::TextureCache::ref();

    auto panel = createPanel(registry, PanelType::GAME_OVER_PANEL, logicalWidth, 0, logicalWidth, logicalHeight);

    auto ribbonHandle = textureCache.handle("gui/ribbon");
    createSprite(registry, panel, ribbonHandle, 220);

    auto popupHandle = textureCache.handle("gui/popup");
    auto popup = createSprite(registry, panel, popupHandle, 210);
    setPos(registry, popup, ribbonHandle->width() / 2 - popupHandle->width() / 2, ribbonHandle->height() / 2);
    const auto &popupTransform = registry.get<Transform>(popup);

    auto menuHandle = textureCache.handle("bt/empty");
    auto menu = createUIButton(registry, panel, menuHandle, UIAction::MENU, 220);
    setPos(registry, menu, popupTransform.x + popupHandle->width() / 4 - menuHandle->width() / 2, ribbonHandle->height() / 2 + popupHandle->height() - 2 * menuHandle->height() / 3);
    setSpriteSize(registry, menu, buttonSize, buttonSize);

    auto shareHandle = textureCache.handle("bt/empty");
    auto share = createUIButton(registry, panel, shareHandle, UIAction::SHARE, 220);
    setPos(registry, share, popupTransform.x + popupHandle->width() / 2 - shareHandle->width() / 2, popupTransform.y + popupHandle->height() - 2 * shareHandle->height() / 3);
    setSpriteSize(registry, share, buttonSize, buttonSize);

    auto reloadHandle = textureCache.handle("bt/empty");
    auto reload = createUIButton(registry, panel, reloadHandle, UIAction::RELOAD, 220);
    setPos(registry, reload, popupTransform.x + 3 * popupHandle->width() / 4 - reloadHandle->width() / 2, popupTransform.y + popupHandle->height() - 2 * reloadHandle->height() / 3);
    setSpriteSize(registry, reload, buttonSize, buttonSize);
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


void createSmashButtons(Registry &registry) {
    auto panel = createPanel(registry, PanelType::SMASH_BUTTONS_PANEL, logicalWidth + 96, logicalHeight / 2 - 368, 96, 736);

    auto addButton = [panel, &registry](FaceType type, int idx) {
        auto handle = toHandle(type);
        auto button = createSmashButton(registry, panel, handle, type, 220);
        setPos(registry, button, 0, idx * (96 + 32));
        setSpriteSize(registry, button, 96, 96);
        setBoundingBox(registry, button, 0, 0, 96, 96);
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
    auto offset = 0;

    auto fpsHandle = textureCache.handle("str/debug/fps");
    auto fpsDebugEntity = createHUD(registry, fpsHandle, 255);
    auto &fpsDebug = registry.attach<FPSDebug>(fpsDebugEntity);
    setPos(registry, fpsDebugEntity, offset + .2f * fpsHandle->width(), logicalHeight - 1.2f * fpsHandle->height());
    offset = registry.get<Transform>(fpsDebugEntity).x + 1.2f * fpsHandle->width();

    for(auto i = 0u; i < std::extent<decltype(FPSDebug::entities)>::value; ++i) {
        fpsDebug.entities[i] = createHUD(registry, sym0Handle, 255);
        setPos(registry, fpsDebug.entities[i], offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
        offset += sym0Handle->width();
    }

    auto timeHandle = textureCache.handle("str/debug/time");
    auto timeDebugEntity = createHUD(registry, timeHandle, 255);
    auto &timeDebug = registry.attach<TimeDebug>(timeDebugEntity);
    setPos(registry, timeDebugEntity, offset + .5f * timeHandle->width(), logicalHeight - .2f * fpsHandle->height() - timeHandle->height());
    offset = registry.get<Transform>(timeDebugEntity).x + 1.2f * timeHandle->width();

    timeDebug.entities[0] = createHUD(registry, sym0Handle, 255);
    setPos(registry, timeDebug.entities[0], offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
    offset += sym0Handle->width();

    timeDebug.entities[1] = createHUD(registry, sym0Handle, 255);
    setPos(registry, timeDebug.entities[1], offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
    offset += sym0Handle->width();

    auto timeDebugDotEntity = createHUD(registry, textureCache.handle("str/debug/."), 255);
    setPos(registry, timeDebugDotEntity, offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
    offset += sym0Handle->width();

    timeDebug.entities[2] = createHUD(registry, sym0Handle, 255);
    setPos(registry, timeDebug.entities[2], offset, logicalHeight - .2f * fpsHandle->height() - sym0Handle->height());
    offset += sym0Handle->width();
}


}
