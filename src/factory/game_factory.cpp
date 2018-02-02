#include <type_traits>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "ui_factory.h"
#include "game_factory.h"


namespace gamee {


void createExitPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::EXIT, 0, logicalHeight, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto exitHandle = textureCache.handle("str/exit");
    auto exit = createSprite(registry, parent, exitHandle, 20);
    setPos(registry, exit, (panel.w - exitHandle->width()) / 2, (panel.h - 3 * exitHandle->height()) / 2);

    auto acceptHandle = textureCache.handle("img/ok");
    auto acceptButton = createUIButton(registry, parent, acceptHandle, UIAction::EXIT, 20);
    setPos(registry, acceptButton, (panel.w - 3 * acceptHandle->width()) / 2, panel.h / 2);

    auto rejectHandle = textureCache.handle("img/ko");
    auto rejectButton = createUIButton(registry, parent, rejectHandle, UIAction::MENU, 20);
    setPos(registry, rejectButton, (panel.w + acceptHandle->width()) / 2, panel.h / 2);
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
    auto parent = createPanel(registry, PanelType::BACKGROUND_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    auto handle = textureCache.handle("bg/mono");
    createSprite(registry, parent, handle, 120);
}


void createBackgroundBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto parent = createPanel(registry, PanelType::BACKGROUND_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 2);
    auto handle = textureCache.handle("bg/mono");
    createSprite(registry, parent, handle, 120);
}


void createMenuTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MENU_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    SDLTextureHandle handles[4] = {
        textureCache.handle("str/facesmash/red/F"),
        textureCache.handle("str/facesmash/lightblue/A"),
        textureCache.handle("str/facesmash/yellow/C"),
        textureCache.handle("str/facesmash/blue/E")
    };

    const auto y = panel.h - 9 * handles[0]->height() / 8;
    const auto length = std::extent<decltype(handles)>::value;
    auto x = panel.w / 2 - 2 * handles[0]->width();

    for(auto i = 0u; i < length; ++i) {
        auto entity = createSprite(registry, parent, handles[i], 150);
        setPos(registry, entity, x, y);
        x += handles[i]->width();
    }

    auto trainingHandle = textureCache.handle("img/training");
    auto trainingButton = createUIButton(registry, parent, trainingHandle, UIAction::TRAINING, 150);
    setPos(registry, trainingButton, (panel.w / 2 - trainingHandle->width()) / 2, 2 * panel.h / 5);

    auto playHandle = textureCache.handle("img/play");
    auto playButton = createUIButton(registry, parent, playHandle, UIAction::THE_GAME, 150);
    setPos(registry, playButton, (panel.w - playHandle->width()) / 2, 2 * panel.h / 5 - playHandle->height() / 2);

    auto medalHandle = textureCache.handle("img/medal");
    auto medalButton = createUIButton(registry, parent, medalHandle, UIAction::ACHIEVEMENTS, 150);
    setPos(registry, medalButton, (3 * panel.w / 2 - medalHandle->width()) / 2, 2 * panel.h / 5);
}


void createMenuBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MENU_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    SDLTextureHandle handles[5] = {
        textureCache.handle("str/facesmash/green/S"),
        textureCache.handle("str/facesmash/lightblue/M"),
        textureCache.handle("str/facesmash/yellow/A"),
        textureCache.handle("str/facesmash/red/S"),
        textureCache.handle("str/facesmash/green/H")
    };

    const auto y = handles[0]->height() / 8;
    const auto length = std::extent<decltype(handles)>::value;
    auto x = (panel.w - 5 * handles[0]->width()) / 2;

    for(auto i = 0u; i < length; ++i) {
        auto entity = createSprite(registry, parent, handles[i], 150);
        setPos(registry, entity, x, y);
        x += handles[i]->width();
    }

    auto infoHandle = textureCache.handle("img/info");
    auto infoButton = createUIButton(registry, parent, infoHandle, UIAction::CREDITS, 150);
    setPos(registry, infoButton, (panel.w / 2 - infoHandle->width()) / 2, 3 * panel.h / 5 - infoHandle->height());

    auto supportHandle = textureCache.handle("img/support");
    auto supportButton = createUIButton(registry, parent, supportHandle, UIAction::SUPPORT, 150);
    setPos(registry, supportButton, (panel.w - supportHandle->width()) / 2, 3 * panel.h / 5 - supportHandle->height() / 2);

    auto gearHandle = textureCache.handle("img/gear");
    auto gearButton = createUIButton(registry, parent, gearHandle, UIAction::SETTINGS, 150);
    setPos(registry, gearButton, (3 * panel.w / 2 - gearHandle->width()) / 2, 3 * panel.h / 5 - gearHandle->height());
}


void createCreditsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::CREDITS, logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/credits");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto backHandle = textureCache.handle("img/prev");
    auto backButton = createUIButton(registry, parent, backHandle, UIAction::MENU, 150);
    setPos(registry, backButton, backHandle->width() / 2, panel.h - 3 * backHandle->height() / 2);
}


void createSupportPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SUPPORT, logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/support");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto backHandle = textureCache.handle("img/prev");
    auto backButton = createUIButton(registry, parent, backHandle, UIAction::MENU, 150);
    setPos(registry, backButton, backHandle->width() / 2, panel.h - 3 * backHandle->height() / 2);
}


void refreshSupportPanel(Registry &registry) {
    // TODO
}


void createSettingsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto &muxer = Locator::AvRecorder::ref();
    auto &audio = Locator::Audio::ref();

    auto parent = createPanel(registry, PanelType::SETTINGS, logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/settings");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto audioStrHandle = textureCache.handle("str/audio");
    auto audioStrEntity = createSprite(registry, parent, audioStrHandle, 150);
    setPos(registry, audioStrEntity, (panel.w - audioStrHandle->width()) / 2, panel.h / 4);

    auto videoStrHandle = textureCache.handle("str/video");
    auto videoStrEntity = createSprite(registry, parent, videoStrHandle, 150);
    setPos(registry, videoStrEntity, (panel.w - videoStrHandle->width()) / 2, 3 * panel.h / 5);

    auto audioHandle = audio.isMute() ? textureCache.handle("img/audio/off") : textureCache.handle("img/audio/on");
    auto audioButton = createUIButton(registry, parent, audioHandle, UIAction::SWITCH_AUDIO, 150);
    setPos(registry, audioButton, (panel.w - audioHandle->width()) / 2, (3 * panel.h / 4 - audioHandle->height()) / 2);

    auto videoHandle = muxer.available() ? textureCache.handle("img/video/on") : textureCache.handle("img/video/off");
    auto videoButton = createUIButton(registry, parent, videoHandle, UIAction::SWITCH_VIDEO, 150);
    setPos(registry, videoButton, (panel.w - videoHandle->width()) / 2, 3 * panel.h / 5 + panel.h/ 8 - videoHandle->height() / 2);

    auto backHandle = textureCache.handle("img/prev");
    auto backButton = createUIButton(registry, parent, backHandle, UIAction::MENU, 150);
    setPos(registry, backButton, backHandle->width() / 2, panel.h - 3 * backHandle->height() / 2);
}


void createAchievementsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::ACHIEVEMENTS, logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/achievements");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto backHandle = textureCache.handle("img/prev");
    auto backButton = createUIButton(registry, parent, backHandle, UIAction::MENU, 150);
    setPos(registry, backButton, backHandle->width() / 2, panel.h - 3 * backHandle->height() / 2);
}


void refreshAchievementsPanel(Registry &registry) {
    // TODO
}


void createTutorialTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TUTORIAL_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto addButton = [&](TextureCache::resource_type face, int idx) {
        auto emojiHandle = textureCache.handle(face);
        const auto buttonOffset = (panel.w - (numberOfFaces * emojiHandle->width() + (numberOfFaces - 1) * 10)) / 2;
        auto emoji = createSprite(registry, parent, emojiHandle, 160);
        setSpriteSize(registry, emoji, 3 * emojiHandle->width() / 5, 3 * emojiHandle->height() / 5);
        setPos(registry, emoji, buttonOffset + idx * (emojiHandle->width() + 10), panel.h - 3 * emojiHandle->height() / 2);
    };

    addButton("face/angry", 0);
    addButton("face/disgusted", 1);
    addButton("face/happy", 2);
    addButton("face/surprised", 3);
    addButton("face/fearful", 4);
    addButton("face/sad", 5);

    auto tutorialHandle = textureCache.handle("str/tutorial/face");
    auto tutorial = createSprite(registry, parent, tutorialHandle, 150);
    setPos(registry, tutorial, (panel.w - tutorialHandle->width()) / 2, panel.h - 3 * tutorialHandle->height());
}


void createTutorialBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TUTORIAL_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto addButton = [&](TextureCache::resource_type face, int idx) {
        auto emojiHandle = textureCache.handle(face);
        const auto buttonOffset = (panel.w - (numberOfItems * emojiHandle->width() + (numberOfItems - 1) * 10)) / 2;
        auto emoji = createSprite(registry, parent, emojiHandle, 160);
        setSpriteSize(registry, emoji, 3 * emojiHandle->width() / 5, 3 * emojiHandle->height() / 5);
        setPos(registry, emoji, buttonOffset + idx * (emojiHandle->width() + 10), emojiHandle->height() / 2);
    };

    addButton("item/speed_up", 0);
    addButton("item/slow_down", 1);
    addButton("item/fountain", 2);

    auto tutorialHandle = textureCache.handle("str/tutorial/touch");
    auto tutorial = createSprite(registry, parent, tutorialHandle, 150);
    setPos(registry, tutorial, (panel.w - tutorialHandle->width()) / 2, 2 * tutorialHandle->height());
}


void createGameTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto sym0Handle = toStrHud(0);
    auto symEmptyHandle = textureCache.handle("str/hud/ ");
    auto scoreHandle = textureCache.handle("str/hud/score");
    auto timerHandle = textureCache.handle("str/hud/timer");

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

    auto menuHandle = textureCache.handle("img/menu");
    auto menuButton = createUIButton(registry, parent, menuHandle, UIAction::MENU, 150);
    setPos(registry, menuButton, (panel.w / 2 - menuHandle->width()) / 2, (3 * panel.h / 2 - menuHandle->height()) / 2);

    auto playHandle = textureCache.handle("img/play");
    auto playButton = createUIButton(registry, parent, playHandle, UIAction::RELOAD, 150);
    setPos(registry, playButton, (panel.w - playHandle->width()) / 2, (3 * panel.h / 2 - playHandle->height()) / 2);

    auto saveHandle = textureCache.handle("img/save");
    auto saveButton = createUIButton(registry, parent, saveHandle, UIAction::SAVE, 150);
    setPos(registry, saveButton, (3 * panel.w / 2 - saveHandle->width()) / 2, (3 * panel.h / 2 - saveHandle->height()) / 2);
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
        auto handle = toHandle(type);
        const auto w = 3 * handle->width() / 4;
        const auto h = 3 * handle->height() / 4;
        auto button = createSmashButton(registry, handle, type, 220);
        setPos(registry, button, logicalWidth, logicalHeight / 2 + h * (idx - 3));
        setSpriteSize(registry, button, w, h);
        setBoundingBox(registry, button, 0, 0, w, h);
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

    auto symDotHandle = textureCache.handle("str/debug/.");
    auto sym0Handle = toStrDebug(0);

    auto &debug = registry.assign<DebugInfo>(registry.create());

    offset = sym0Handle->width();

    auto fpsHandle = textureCache.handle("str/debug/fps");
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

    auto timeHandle = textureCache.handle("str/debug/time");
    auto timeEntity = createHUD(registry, timeHandle, 255);
    offset -= timeHandle->width() + sym0Handle->width();
    setPos(registry, timeEntity, offset, logicalHeight - timeHandle->height());
}


}
