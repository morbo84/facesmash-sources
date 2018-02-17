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

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_2_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_2_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_2_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_2_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto exitHandle = textureCache.handle("str/exit");
    auto exit = createSprite(registry, parent, exitHandle, 20);
    setPos(registry, exit, (panel.w - exitHandle->width()) / 2, (panel.h - 3 * exitHandle->height()) / 2);

    auto acceptButton = createUIButton(registry, parent, UIAction::EXIT, 20);
    const auto &acceptSprite = registry.get<Sprite>(acceptButton);
    setSpriteSize(registry, acceptButton, 3 * acceptSprite.w / 4, 3 * acceptSprite.h / 4);
    setPos(registry, acceptButton, (panel.w - 3 * acceptSprite.w) / 2, panel.h / 2);
    setBoundingBox(registry, acceptButton, acceptSprite.w, acceptSprite.h);

    auto rejectButton = createUIButton(registry, parent, UIAction::MENU, 20);
    const auto &rejectSprite = registry.get<Sprite>(rejectButton);
    setSpriteSize(registry, rejectButton, 3 * rejectSprite.w / 4, 3 * rejectSprite.h / 4);
    setPos(registry, rejectButton, (panel.w + rejectSprite.w) / 2, panel.h / 2);
    setBoundingBox(registry, rejectButton, rejectSprite.w, rejectSprite.h);
}


void createSplashScreenPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SPLASH_SCREEN, -logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto gameeHandle = textureCache.handle("str/gamee");
    auto gamee = createSprite(registry, parent, gameeHandle, 20);
    setPos(registry, gamee, (panel.w - gameeHandle->width()) / 2, (panel.h - gameeHandle->height()) / 2);
    registry.assign<BoundingBox>(gamee, 0, 0, gameeHandle->width(), gameeHandle->height());
    registry.assign<UIButton>(gamee, static_cast<bool>(gamee), UIAction::EASTER_EGG);
    registry.assign<FadeAnimation>(gamee, 0, 255, 2000_ui32);
    registry.get<Renderable>(gamee).alpha = 0;
}


void createBackgroundTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    const auto width = logicalWidth;
    const auto height = logicalHeight / 2;

    auto parent = createPanel(registry, PanelType::BACKGROUND_TOP, 0, -logicalHeight / 2, width, height);
    const auto &panel = registry.get<Panel>(parent);

    auto handle = textureCache.handle("ui/win_hud");
    auto bg = createSprite(registry, parent, handle, 120);
    setSpriteGeometry(registry, bg, 0, 0, width, height, 1_ui8);
    setSpriteSize(registry, bg, width, height);

    auto border = createBoxBorder(registry, parent, BoxBorderType::BOX_1_TOP, width, 140);
    setPos(registry, border, 0, height - 5);
    registry.get<Renderable>(border).alpha = 120;

    auto createCloseButton = [&registry, &panel, parent](auto x){
        auto closeButton = createPopupUIButton(registry, parent, UIAction::MENU_CLOSE_DOWN, 180);
        auto &closeSprite = registry.get<Sprite>(closeButton);
        auto &closeUIButton = registry.get<UIButton>(closeButton);
        closeUIButton.w = closeSprite.w = 2 * closeSprite.w / 3;
        closeUIButton.h = closeSprite.h = 2 * closeSprite.h / 3;
        setPos(registry, closeButton, x(closeSprite), panel.h - 3 * closeSprite.h / 8);
        setBoundingBox(registry, closeButton, closeUIButton.w, closeUIButton.h);
        setSpriteSize(registry, closeButton, 0, 0);
    };

    createCloseButton([&panel](const auto &sprite) { return panel.w - 5 * sprite.w / 8; });
    createCloseButton([&panel](const auto &sprite) { return 5 * sprite.w / 8; });
}


void createBackgroundBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    const auto width = logicalWidth;
    const auto height = logicalHeight / 2;

    auto parent = createPanel(registry, PanelType::BACKGROUND_BOTTOM, 0, logicalHeight, width, height);
    const auto &panel = registry.get<Panel>(parent);

    auto handle = textureCache.handle("ui/win_hud");
    auto bg = createSprite(registry, parent, handle, 120);
    setSpriteGeometry(registry, bg, 0, height, width, height, 1_ui8);
    setSpriteSize(registry, bg, width, height);

    auto border = createBoxBorder(registry, parent, BoxBorderType::BOX_1_BOTTOM, width, 140);
    registry.get<Renderable>(border).alpha = 120;

    auto createCloseButton = [&registry, &panel, parent](auto x){
        auto closeButton = createPopupUIButton(registry, parent, UIAction::MENU_CLOSE_UP, 180);
        auto &closeSprite = registry.get<Sprite>(closeButton);
        auto &closeUIButton = registry.get<UIButton>(closeButton);
        closeUIButton.w = closeSprite.w = 2 * closeSprite.w / 3;
        closeUIButton.h = closeSprite.h = 2 * closeSprite.h / 3;
        setPos(registry, closeButton, x(closeSprite), 3 * closeSprite.h / 8);
        setBoundingBox(registry, closeButton, closeUIButton.w, closeUIButton.h);
        setSpriteSize(registry, closeButton, 0, 0);
    };

    createCloseButton([&panel](const auto &sprite) { return panel.w - 5 * sprite.w / 8; });
    createCloseButton([&panel](const auto &sprite) { return 5 * sprite.w / 8; });
}


void createMenuTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MENU_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/face");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, panel.h - 9 * titleHandle->height() / 8);

    auto supportButton = createPopupUIButton(registry, parent, UIAction::SUPPORT, 150);
    const auto &supportSprite = registry.get<Sprite>(supportButton);
    setPos(registry, supportButton, (panel.w / 2 - supportSprite.w) / 2, 2 * panel.h / 5);

    auto playButton = createPopupUIButton(registry, parent, UIAction::THE_GAME, 150);
    const auto &playSprite = registry.get<Sprite>(playButton);
    setPos(registry, playButton, (panel.w - playSprite.w) / 2, 2 * panel.h / 5 - playSprite.h / 2);

    auto trainingButton = createPopupUIButton(registry, parent, UIAction::TRAINING, 150);
    const auto &trainingSprite = registry.get<Sprite>(trainingButton);
    setPos(registry, trainingButton, (3 * panel.w / 2 - trainingSprite.w) / 2, 2 * panel.h / 5);

    auto settingsButton = createPopupUIButton(registry, parent, UIAction::SETTINGS, 150);
    const auto &settingsSprite = registry.get<Sprite>(settingsButton);
    setPopupUIButtonSize(registry, settingsButton, 2 * settingsSprite.w / 3, 2 * settingsSprite.h / 3);
    setSpriteSize(registry, settingsButton, 2 * settingsSprite.w / 3, 2 * settingsSprite.h / 3);
    setPos(registry, settingsButton, panel.w - 9 * settingsSprite.w / 8, settingsSprite.h / 8);

    auto loginButton = createPopupUIButton(registry, parent, UIAction::LOGIN, 150);
    const auto &loginSprite = registry.get<Sprite>(loginButton);
    setPopupUIButtonSize(registry, loginButton, 2 * loginSprite.w / 3, 2 * loginSprite.h / 3);
    setSpriteSize(registry, loginButton, 2 * loginSprite.w / 3, 2 * loginSprite.h / 3);
    setPos(registry, loginButton, loginSprite.w / 8, loginSprite.h / 8);
}


void createMenuBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MENU_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/smash");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 8);

    auto achievementsButton = createPopupUIButton(registry, parent, UIAction::ACHIEVEMENTS, 150);
    const auto &achievementsSprite = registry.get<Sprite>(achievementsButton);
    setPos(registry, achievementsButton, (panel.w / 2 - achievementsSprite.w) / 2, 3 * panel.h / 5 - achievementsSprite.h);

    auto creditsButton = createPopupUIButton(registry, parent, UIAction::CREDITS, 150);
    const auto &creditsSprite = registry.get<Sprite>(creditsButton);
    setPos(registry, creditsButton, (panel.w - creditsSprite.w) / 2, 3 * panel.h / 5 - creditsSprite.h / 2);

    auto leaderboardButton = createPopupUIButton(registry, parent, UIAction::LEADERBOARD, 150);
    const auto &leaderboardSprite = registry.get<Sprite>(leaderboardButton);
    setPos(registry, leaderboardButton, (3 * panel.w / 2 - leaderboardSprite.w) / 2, 3 * panel.h / 5 - leaderboardSprite.h);
}


void createCreditsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::CREDITS, logicalWidth, 0, logicalWidth, 3 * logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_3_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_3_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_3_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_3_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto titleHandle = textureCache.handle("str/credits");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);
}


void createSupportPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SUPPORT, logicalWidth, 0, logicalWidth, 3 * logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_4_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_4_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_4_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_4_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto titleHandle = textureCache.handle("str/support");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);
}


void refreshSupportPanel(Registry &registry) {
    // TODO
}


void createSettingsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SETTINGS, logicalWidth, 0, logicalWidth, 5 * logicalHeight / 8);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_5_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_5_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_5_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_5_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

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
}


void createTutorialTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TUTORIAL_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto addButton = [&](FaceType type, int idx) {
        auto entity = createFaceBlueprint(registry, type, 160);
        const auto &sprite = registry.get<Sprite>(entity);

        const auto width = 3 * sprite.w / 5;
        const auto height = 3 * sprite.h / 5;

        const auto buttonOffset = (panel.w - (numberOfFaces * width + (numberOfFaces - 1) * 10)) / 2;

        setPos(registry, entity, buttonOffset + idx * (width + 10), panel.h - 3 * height / 2);
        setSpriteSize(registry, entity, width, height);
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

        const auto width = 3 * sprite.w / 5;
        const auto height = 3 * sprite.h / 5;

        const auto buttonOffset = (panel.w - (numberOfItems * width + (numberOfItems - 1) * 10)) / 2;

        setPos(registry, entity, buttonOffset + idx * (width + 10), height / 2);
        setSpriteSize(registry, entity, width, height);
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

    auto sym0Handle = toStrSmallHandle(0);
    auto symEmptyHandle = textureCache.handle("str/small/ ");
    auto scoreHandle = textureCache.handle("str/game/score");
    auto timerHandle = textureCache.handle("str/game/timer");

    auto offset = 0;

    auto parent = createPanel(registry, PanelType::THE_GAME_TOP, 0, -logicalHeight / 8, logicalWidth, logicalHeight / 8);
    const auto &panel = registry.get<Panel>(parent);

    auto playerScoreEntity = createHUD(registry, parent, scoreHandle, 160);
    auto &playerScoreObserver = registry.assign<PlayerScoreObserver>(playerScoreEntity);
    setPos(registry, playerScoreEntity, offset + .1f * scoreHandle->width(), .4f * scoreHandle->height());;
    offset = registry.get<Transform>(playerScoreEntity).x + 1.1f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(PlayerScoreObserver::entities)>::value; ++i) {
        auto handle = i ? symEmptyHandle : sym0Handle;
        playerScoreObserver.entities[i] = createHUD(registry, parent, handle, 160);
        setPos(registry, playerScoreObserver.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    offset = panel.w - .2f * scoreHandle->width() - 2 * sym0Handle->width() - timerHandle->width();

    auto timerEntity = createHUD(registry, parent, timerHandle, 160);
    auto &timerObserver = registry.assign<TimerObserver>(timerEntity);
    setPos(registry, timerEntity, offset, .4f * scoreHandle->height());;
    offset = registry.get<Transform>(timerEntity).x + timerHandle->width() + .1f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(TimerObserver::entities)>::value; ++i) {
        auto handle = i ? symEmptyHandle : sym0Handle;
        timerObserver.entities[i] = createHUD(registry, parent, handle, 160);
        setPos(registry, timerObserver.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }
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
    if(registry.has<PlayerScore>()) {
        auto &textureCache = Locator::TextureCache::ref();
        entity_type parent{};

        for(auto entity: registry.view<Panel>()) {
            if(registry.get<Panel>(entity).type == PanelType::GAME_OVER) {
                parent = entity;
                break;
            }
        }

        const auto &playerScore = registry.get<PlayerScore>();
        const auto &panel = registry.get<Panel>(parent);

        auto printHit = [&](FaceType type, int PlayerScore:: *member, int xOffset, auto initYOffset) {
            auto faceEntity = createFaceBlueprint(registry, type, 150);
            const auto &faceSprite = registry.get<Sprite>(faceEntity);
            setSpriteSize(registry, faceEntity, faceSprite.w / 2, faceSprite.h / 2);
            auto yOffset = panel.h / 2 + initYOffset(faceSprite);
            setPos(registry, faceEntity, xOffset, yOffset);
            registry.assign<ExpiringContent>(faceEntity);
            registry.get<Transform>(faceEntity).parent = parent;
            yOffset += faceSprite.h / 3;
            xOffset += faceSprite.w;

            auto xEntity = createSprite(registry, parent, textureCache.handle("str/small/x"), 150);
            const auto &xSprite = registry.get<Sprite>(xEntity);
            setPos(registry, xEntity, xOffset+ xSprite.w / 2, yOffset);
            registry.assign<ExpiringContent>(xEntity);
            xOffset += 2 * xSprite.w;

            auto res0Entity = createSprite(registry, parent, toStrSmallHandle((playerScore.*member) / 10), 150);
            const auto &res0Sprite = registry.get<Sprite>(res0Entity);
            setPos(registry, res0Entity, xOffset, yOffset);
            registry.assign<ExpiringContent>(res0Entity);
            xOffset += res0Sprite.w;

            auto res1Entity = createSprite(registry, parent, toStrSmallHandle((playerScore.*member) % 10), 150);
            const auto &res1Sprite = registry.get<Sprite>(res1Entity);
            setPos(registry, res1Entity, xOffset, yOffset);
            registry.assign<ExpiringContent>(res1Entity);
            xOffset += res1Sprite.w;
        };

        // first line
        printHit(FaceType::HAPPY, &PlayerScore::hitHappy, panel.w / 6, [](const auto &sprite) { return -17 * sprite.h / 4; });
        printHit(FaceType::ANGRY, &PlayerScore::hitAngry, 5 * panel.w / 9, [](const auto &sprite) { return -17 * sprite.h / 4; });
        // second line
        printHit(FaceType::SAD, &PlayerScore::hitSad, panel.w / 6, [](const auto &sprite) { return -12 * sprite.h / 4; });
        printHit(FaceType::SURPRISED, &PlayerScore::hitSurprised, 5 * panel.w / 9, [](const auto &sprite) { return -12 * sprite.h / 4; });
        // third line
        printHit(FaceType::DISGUSTED, &PlayerScore::hitDisgusted, panel.w / 6, [](const auto &sprite) { return -7 * sprite.h / 4; });
        printHit(FaceType::FEARFUL, &PlayerScore::hitFearful, 5 * panel.w / 9, [](const auto &sprite) { return -7 * sprite.h / 4; });

        auto scoreEntity = createSprite(registry, parent, textureCache.handle("str/score"), 150);
        const auto &scoreSprite = registry.get<Sprite>(scoreEntity);
        setPos(registry, scoreEntity, (panel.w - scoreSprite.w) / 2, panel.h / 2 + 2 * scoreSprite.h / 3);
        registry.assign<ExpiringContent>(scoreEntity);

        auto printScore = [&](int score, auto getXOffset) {
            auto entity = createSprite(registry, parent, toStrNormalHandle(score), 150);
            const auto &sprite = registry.get<Sprite>(entity);
            setPos(registry, entity, panel.w / 2 + getXOffset(sprite), panel.h / 2 + 2 * sprite.h);
            registry.assign<ExpiringContent>(entity);
        };

        auto score = playerScore.score;
        printScore(score % 10, [](const auto &sprite) { return 3 * sprite.w / 2; });
        score /= 10;
        printScore(score % 10, [](const auto &sprite) { return sprite.w / 2; });
        score /= 10;
        printScore(score % 10, [](const auto &sprite) { return -sprite.w / 2; });
        score /= 10;
        printScore(score % 10, [](const auto &sprite) { return -3 * sprite.w / 2; });
        score /= 10;
        printScore(score % 10, [](const auto &sprite) { return -5 * sprite.w / 2; });
    }
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
