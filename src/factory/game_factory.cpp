#include <type_traits>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../factory/play_factory.h"
#include "../locator/locator.hpp"
#include "common.h"
#include "ui_factory.h"
#include "game_factory.h"


namespace gamee {


void createExitPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::EXIT, 0, logicalHeight, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto exitHandle = textureCache.handle("str/exit");
    auto exit = createSprite(registry, parent, exitHandle, 20);
    setPos(registry, exit, (panel.w - exitHandle->width()) / 2, (panel.h - 3 * exitHandle->height()) / 2);

    auto acceptButton = createUIButton(registry, parent, UIAction::EXIT, 20);
    const auto &acceptSprite = registry.get<Sprite>(acceptButton);
    setPos(registry, acceptButton, (panel.w - 3 * acceptSprite.w) / 2, panel.h / 2);

    auto rejectButton = createUIButton(registry, parent, UIAction::MENU, 20);
    const auto &rejectSprite = registry.get<Sprite>(rejectButton);
    setPos(registry, rejectButton, (panel.w + rejectSprite.w) / 2, panel.h / 2);
}


void createSplashScreenPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SPLASH_SCREEN, -logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto gameeHandle = textureCache.handle("str/gamee");
    auto gamee = createSprite(registry, parent, gameeHandle, 20);
    setPos(registry, gamee, (panel.w - gameeHandle->width()) / 2, (panel.h - gameeHandle->height()) / 2);
    registry.assign<BoundingBox>(gamee, 0, 0, gameeHandle->width(), gameeHandle->height());
    registry.assign<UIButton>(gamee, gamee, UIAction::EASTER_EGG);
    registry.assign<FadeAnimation>(gamee, 0, 255, 2000_ui32);
    registry.get<Renderable>(gamee).alpha = 0;
}


void createBackgroundTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    const auto width = logicalWidth;
    const auto height = logicalHeight / 2;
    auto parent = createPanel(registry, PanelType::BACKGROUND_TOP, 0, -logicalHeight / 2, width, height);
    auto handle = textureCache.handle("ui/win_hud");
    auto entity = createSprite(registry, parent, handle, 120);
    setSpriteGeometry(registry, entity, 0, 0, width, height, 1, 1);
    setSpriteSize(registry, entity, width, height);
}


void createBackgroundBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    const auto width = logicalWidth;
    const auto height = logicalHeight / 2;
    auto parent = createPanel(registry, PanelType::BACKGROUND_BOTTOM, 0, logicalHeight, width, height);
    auto handle = textureCache.handle("ui/win_hud");
    auto entity = createSprite(registry, parent, handle, 120);
    setSpriteGeometry(registry, entity, 0, 0, width, height, 1, 1);
    setSpriteSize(registry, entity, width, height);
}


void createMenuTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MENU_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/face");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, panel.h - 9 * titleHandle->height() / 8);

    auto trainingButton = createUIButton(registry, parent, UIAction::TRAINING, 150);
    const auto &trainingSprite = registry.get<Sprite>(trainingButton);
    setPos(registry, trainingButton, (panel.w / 2 - trainingSprite.w) / 2, 2 * panel.h / 5);

    auto playButton = createUIButton(registry, parent, UIAction::THE_GAME, 150);
    const auto &playSprite = registry.get<Sprite>(playButton);
    setPos(registry, playButton, (panel.w - playSprite.w) / 2, 2 * panel.h / 5 - playSprite.h / 2);

    auto medalButton = createUIButton(registry, parent, UIAction::ACHIEVEMENTS, 150);
    const auto &medalSprite = registry.get<Sprite>(medalButton);
    setPos(registry, medalButton, (3 * panel.w / 2 - medalSprite.w) / 2, 2 * panel.h / 5);
}


void createMenuBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MENU_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/smash");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 8);

    auto infoButton = createUIButton(registry, parent, UIAction::CREDITS, 150);
    const auto &infoSprite = registry.get<Sprite>(infoButton);
    setPos(registry, infoButton, (panel.w / 2 - infoSprite.w) / 2, 3 * panel.h / 5 - infoSprite.h);

    auto supportButton = createUIButton(registry, parent, UIAction::SUPPORT, 150);
    const auto &supportSprite = registry.get<Sprite>(supportButton);
    setPos(registry, supportButton, (panel.w - supportSprite.w) / 2, 3 * panel.h / 5 - supportSprite.h / 2);

    auto settingsButton = createUIButton(registry, parent, UIAction::SETTINGS, 150);
    const auto &settingsSprite = registry.get<Sprite>(settingsButton);
    setPos(registry, settingsButton, (3 * panel.w / 2 - settingsSprite.w) / 2, 3 * panel.h / 5 - settingsSprite.h);
}


void createCreditsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::CREDITS, logicalWidth, 0, logicalWidth, 3 * logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/credits");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto menuButton = createUIButton(registry, parent, UIAction::MENU, 20);
    const auto &menuSprite = registry.get<Sprite>(menuButton);
    setPos(registry, menuButton, menuSprite.w / 2, panel.h - 3 * menuSprite.h / 2);
}


void createSupportPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SUPPORT, logicalWidth, 0, logicalWidth, 3 * logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/support");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto menuButton = createUIButton(registry, parent, UIAction::MENU, 20);
    const auto &menuSprite = registry.get<Sprite>(menuButton);
    setPos(registry, menuButton, menuSprite.w / 2, panel.h - 3 * menuSprite.h / 2);
}


void refreshSupportPanel(Registry &registry) {
    // TODO
}


void createSettingsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SETTINGS, logicalWidth, 0, logicalWidth, 3 * logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/settings");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto audioStrHandle = textureCache.handle("str/audio");
    auto audioStrEntity = createSprite(registry, parent, audioStrHandle, 20);
    setPos(registry, audioStrEntity, (panel.w - audioStrHandle->width()) / 2, panel.h / 4);

    auto videoStrHandle = textureCache.handle("str/video");
    auto videoStrEntity = createSprite(registry, parent, videoStrHandle, 20);
    setPos(registry, videoStrEntity, (panel.w - videoStrHandle->width()) / 2, 3 * panel.h / 5);

    auto audioButton = createUIButton(registry, parent, UIAction::SWITCH_AUDIO, 20);
    const auto &audioSprite = registry.get<Sprite>(audioButton);
    setPos(registry, audioButton, (panel.w - audioSprite.w) / 2, (3 * panel.h / 4 - audioSprite.h) / 2);

    auto videoButton = createUIButton(registry, parent, UIAction::SWITCH_VIDEO, 20);
    const auto &videoSprite = registry.get<Sprite>(videoButton);
    setPos(registry, videoButton, (panel.w - videoSprite.w) / 2, 3 * panel.h / 5 + panel.h/ 8 - videoSprite.h / 2);

    auto menuButton = createUIButton(registry, parent, UIAction::MENU, 20);
    const auto &menuSprite = registry.get<Sprite>(menuButton);
    setPos(registry, menuButton, menuSprite.w / 2, panel.h - 3 * menuSprite.h / 2);
}


void createAchievementsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::ACHIEVEMENTS, logicalWidth, 0, logicalWidth, 3 * logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/achievements");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto menuButton = createUIButton(registry, parent, UIAction::MENU, 20);
    const auto &menuSprite = registry.get<Sprite>(menuButton);
    setPos(registry, menuButton, menuSprite.w / 2, panel.h - 3 * menuSprite.h / 2);
}


void refreshAchievementsPanel(Registry &registry) {
    // TODO
}


void createTutorialTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TUTORIAL_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto addButton = [&](FaceType type, int idx) {
        auto entity = createFaceBlueprint(registry, type, 160);
        const auto &sprite = registry.get<Sprite>(entity);
        const auto buttonOffset = (panel.w - (numberOfFaces * sprite.w + (numberOfFaces - 1) * 10)) / 2;

        setPos(registry, entity, buttonOffset + idx * (sprite.w + 10), panel.h - 3 * sprite.h / 2);
        setSpriteSize(registry, entity, 3 * sprite.w / 5, 3 * sprite.h / 5);
        registry.get<Transform>(entity).parent = parent;
    };

    addButton(FaceType::ANGRY, 0);
    addButton(FaceType::DISGUSTED, 1);
    addButton(FaceType::HAPPY, 2);
    addButton(FaceType::SURPRISED, 3);
    addButton(FaceType::FEARFUL, 4);
    addButton(FaceType::SAD, 5);

    auto tutorialHandle = textureCache.handle("str/tutorial/face");
    auto tutorial = createSprite(registry, parent, tutorialHandle, 150);
    setPos(registry, tutorial, (panel.w - tutorialHandle->width()) / 2, panel.h - 3 * tutorialHandle->height());
}


void createTutorialBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TUTORIAL_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto addButton = [&](ItemType type, int idx) {
        auto entity = createItemBlueprint(registry, type, 160);
        const auto &sprite = registry.get<Sprite>(entity);
        const auto buttonOffset = (panel.w - (numberOfItems * sprite.w + (numberOfItems - 1) * 10)) / 2;

        setPos(registry, entity, buttonOffset + idx * (sprite.w + 10), sprite.h / 2);
        setSpriteSize(registry, entity, 3 * sprite.w / 5, 3 * sprite.h / 5);
        registry.get<Transform>(entity).parent = parent;
    };

    addButton(ItemType::SPEED_UP, 0);
    addButton(ItemType::SLOW_DOWN, 1);
    addButton(ItemType::FOUNTAIN, 2);

    auto tutorialHandle = textureCache.handle("str/tutorial/touch");
    auto tutorial = createSprite(registry, parent, tutorialHandle, 150);
    setPos(registry, tutorial, (panel.w - tutorialHandle->width()) / 2, 2 * tutorialHandle->height());
}


void createGameTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto sym0Handle = toStrHud(0);
    auto symEmptyHandle = textureCache.handle("str/ ");
    auto scoreHandle = textureCache.handle("str/score");
    auto timerHandle = textureCache.handle("str/timer");

    auto offset = 0;

    auto parent = createPanel(registry, PanelType::THE_GAME_TOP, 0, -logicalHeight / 8, logicalWidth, logicalHeight / 8);
    const auto &panel = registry.get<Panel>(parent);

    auto playerScoreEntity = createHUD(registry, parent, scoreHandle, 160);
    auto &playerScore = registry.attach<PlayerScore>(playerScoreEntity);
    setPos(registry, playerScoreEntity, offset + .2f * scoreHandle->width(), .4f * scoreHandle->height());;
    offset = registry.get<Transform>(playerScoreEntity).x + 1.2f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(PlayerScore::entities)>::value; ++i) {
        auto handle = i ? symEmptyHandle : sym0Handle;
        playerScore.entities[i] = createHUD(registry, parent, handle, 160);
        setPos(registry, playerScore.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    offset = panel.w - .4f * scoreHandle->width() - 2 * sym0Handle->width() - timerHandle->width();

    auto gameTimerEntity = createHUD(registry, parent, timerHandle, 160);
    auto &gameTimer = registry.attach<GameTimer>(gameTimerEntity);
    setPos(registry, gameTimerEntity, offset, .4f * scoreHandle->height());;
    offset = registry.get<Transform>(gameTimerEntity).x + timerHandle->width() + .2f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(GameTimer::entities)>::value; ++i) {
        auto handle = i ? symEmptyHandle : sym0Handle;
        gameTimer.entities[i] = createHUD(registry, parent, handle, 160);
        setPos(registry, gameTimer.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    auto rewardHandle = textureCache.handle("str/reward/perfect");
    auto reward = createSprite(registry, parent, rewardHandle, 160);
    setPos(registry, reward, (panel.w - rewardHandle->width()) / 2, panel.h - rewardHandle->height() / 2);
    registry.get<Renderable>(reward).alpha = 0;
    registry.attach<Reward>(reward);
}


void createGameBottomPanel(Registry &registry) {
    createPanel(registry, PanelType::THE_GAME_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 8);
}


void createGameOverPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::GAME_OVER, logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/gameover");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, (panel.h / 2 - titleHandle->height()) / 2);

    auto menuButton = createUIButton(registry, parent, UIAction::MENU, 150);
    const auto &menuSprite = registry.get<Sprite>(menuButton);
    setPos(registry, menuButton, (panel.w / 2 - menuSprite.w) / 2, (3 * panel.h / 2 - menuSprite.h) / 2);

    auto reloadButton = createUIButton(registry, parent, UIAction::RELOAD, 150);
    const auto &reloadSprite = registry.get<Sprite>(reloadButton);
    setPos(registry, reloadButton, (panel.w - reloadSprite.w) / 2, (3 * panel.h / 2 - reloadSprite.h) / 2);

    auto saveButton = createUIButton(registry, parent, UIAction::SAVE, 150);
    const auto &saveSprite = registry.get<Sprite>(saveButton);
    setPos(registry, saveButton, (3 * panel.w / 2 - saveSprite.w) / 2, (3 * panel.h / 2 - saveSprite.h) / 2);
}


void refreshGameOverPanel(Registry &registry) {
    // TODO
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
    auto addButton = [&registry](FaceType type, int idx) {
        auto entity = createSmashButton(registry, type, 220);

        const auto &sprite = registry.get<Sprite>(entity);
        const auto w = 3 * sprite.w / 4;
        const auto h = 3 * sprite.h / 4;

        setPos(registry, entity, logicalWidth, logicalHeight / 2 + h * (idx - 3));
        setBoundingBox(registry, entity, w, h);
        setSpriteSize(registry, entity, w, h);
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

    auto offset = 0;

    auto symDotHandle = textureCache.handle("str/.");
    auto sym0Handle = toStrDebug(0);

    auto &debug = registry.assign<DebugInfo>(registry.create());

    offset = sym0Handle->width();

    auto fpsHandle = textureCache.handle("str/fps");
    auto fpsEntity = createHUD(registry, fpsHandle, 255);
    setPos(registry, fpsEntity, offset, logicalHeight - fpsHandle->height());

    offset += fpsHandle->width() + sym0Handle->width();

    for(auto i = 0u; i < std::extent<decltype(DebugInfo::fps)>::value; ++i) {
        debug.fps[i] = createHUD(registry, sym0Handle, 255);
        setPos(registry, debug.fps[i], offset, logicalHeight - sym0Handle->height());
        offset += sym0Handle->width();
    }

    offset = logicalWidth - sym0Handle->width();

    offset -= sym0Handle->width();
    debug.time[3] = createHUD(registry, sym0Handle, 255);
    setPos(registry, debug.time[3], offset, logicalHeight - sym0Handle->height());

    offset -= symDotHandle->width();
    auto timeDebugDotEntity = createHUD(registry, symDotHandle, 255);
    setPos(registry, timeDebugDotEntity, offset, logicalHeight - symDotHandle->height());

    offset -= sym0Handle->width();
    debug.time[2] = createHUD(registry, sym0Handle, 255);
    setPos(registry, debug.time[2], offset, logicalHeight - sym0Handle->height());

    offset -= sym0Handle->width();
    debug.time[1] = createHUD(registry, sym0Handle, 255);
    setPos(registry, debug.time[1], offset, logicalHeight - sym0Handle->height());

    offset -= sym0Handle->width();
    debug.time[0] = createHUD(registry, sym0Handle, 255);
    setPos(registry, debug.time[0], offset, logicalHeight - sym0Handle->height());

    auto timeHandle = textureCache.handle("str/time");
    auto timeEntity = createHUD(registry, timeHandle, 255);
    offset -= timeHandle->width() + sym0Handle->width();
    setPos(registry, timeEntity, offset, logicalHeight - timeHandle->height());
}


}
