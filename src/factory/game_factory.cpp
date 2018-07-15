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
    setPos(registry, exit, (panel.w - exitHandle->width()) / 2, (panel.h - 4 * exitHandle->height()) / 2);

    auto acceptButton = createUIButton(registry, parent, UIAction::EXIT, 20);
    const auto &acceptSprite = registry.get<Sprite>(acceptButton);
    setSpriteSize(registry, acceptButton, 2 * acceptSprite.w / 3, 2 * acceptSprite.h / 3);
    setPos(registry, acceptButton, (panel.w - 3 * acceptSprite.w) / 2, panel.h / 2 - acceptSprite.h / 5);
    setBoundingBox(registry, acceptButton, acceptSprite.w, acceptSprite.h);

    auto rejectButton = createUIButton(registry, parent, UIAction::CLOSE, 20);
    const auto &rejectSprite = registry.get<Sprite>(rejectButton);
    setSpriteSize(registry, rejectButton, 2 * rejectSprite.w / 3, 2 * rejectSprite.h / 3);
    setPos(registry, rejectButton, (panel.w + rejectSprite.w) / 2, panel.h / 2 - rejectSprite.h / 5);
    setBoundingBox(registry, rejectButton, rejectSprite.w, rejectSprite.h);
}


void createTitleTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TITLE_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/face");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height());
}


void createTitleBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TITLE_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/smash");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, panel.h - 2 * titleHandle->height());
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
        const auto &closeSprite = registry.get<Sprite>(closeButton);
        setSpriteSize(registry, closeButton, 2 * closeSprite.w / 5, 2 * closeSprite.h / 5);
        setPopupUIButtonSize(registry, closeButton, closeSprite.w, closeSprite.h);
        setPos(registry, closeButton, x(closeSprite), panel.h - 3 * closeSprite.h / 8);
        setBoundingBox(registry, closeButton, 3 * closeSprite.w / 2, 3 * closeSprite.h / 2);
        setSpriteSize(registry, closeButton, 0, 0);
    };

    createCloseButton([&panel](const auto &sprite) { return panel.w - 5 * sprite.w / 8; });
    createCloseButton([](const auto &sprite) { return 5 * sprite.w / 8; });
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

    auto createCloseButton = [&registry, parent](auto x){
        auto closeButton = createPopupUIButton(registry, parent, UIAction::MENU_CLOSE_UP, 180);
        const auto &closeSprite = registry.get<Sprite>(closeButton);
        setSpriteSize(registry, closeButton, 2 * closeSprite.w / 5, 2 * closeSprite.h / 5);
        setPopupUIButtonSize(registry, closeButton, closeSprite.w, closeSprite.h);
        setPos(registry, closeButton, x(closeSprite), 3 * closeSprite.h / 8);
        setBoundingBox(registry, closeButton, 3 * closeSprite.w / 2, 3 * closeSprite.h / 2);
        setSpriteSize(registry, closeButton, 0, 0);
    };

    createCloseButton([&panel](const auto &sprite) { return panel.w - 5 * sprite.w / 8; });
    createCloseButton([](const auto &sprite) { return 5 * sprite.w / 8; });
}


void createMenuTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MENU_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/face");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, panel.h - 9 * titleHandle->height() / 8);

    auto supportButton = createPopupUIButton(registry, parent, UIAction::SUPPORT, 150);
    auto &supportSprite = registry.get<Sprite>(supportButton);
    setSpriteSize(registry, supportButton, 4 * supportSprite.w / 5, 4 * supportSprite.h / 5);
    setPopupUIButtonSize(registry, supportButton, supportSprite.w, supportSprite.h);
    setPos(registry, supportButton, (panel.w / 3 - supportSprite.w) / 2, 2 * panel.h / 5);
    setBoundingBox(registry, supportButton, supportSprite.w, supportSprite.h);
    supportSprite.frame = 2;

    auto playButton = createPopupUIButton(registry, parent, UIAction::PLAY, 150);
    const auto &playSprite = registry.get<Sprite>(playButton);
    setPos(registry, playButton, (panel.w - playSprite.w) / 2, 2 * panel.h / 5 - playSprite.h / 2);
    registry.assign<PulseAnimation>(playButton, 10.f, .4f, .1f, 3000_ui32);

    auto trainingButton = createPopupUIButton(registry, parent, UIAction::TRAINING, 150);
    const auto &trainingSprite = registry.get<Sprite>(trainingButton);
    setSpriteSize(registry, trainingButton, 4 * trainingSprite.w / 5, 4 * trainingSprite.h / 5);
    setPopupUIButtonSize(registry, trainingButton, trainingSprite.w, trainingSprite.h);
    setPos(registry, trainingButton, (5 * panel.w / 3 - trainingSprite.w) / 2, 2 * panel.h / 5);
    setBoundingBox(registry, trainingButton, trainingSprite.w, trainingSprite.h);

    auto settingsButton = createPopupUIButton(registry, parent, UIAction::SETTINGS, 150);
    const auto &settingsSprite = registry.get<Sprite>(settingsButton);
    setSpriteSize(registry, settingsButton, 2 * settingsSprite.w / 3, 2 * settingsSprite.h / 3);
    setPopupUIButtonSize(registry, settingsButton, settingsSprite.w, settingsSprite.h);
    setPos(registry, settingsButton, panel.w - 9 * settingsSprite.w / 8, settingsSprite.h / 8);
    setBoundingBox(registry, settingsButton, settingsSprite.w, settingsSprite.h);

    auto loginButton = createPopupUIButton(registry, parent, UIAction::LOGIN, 150);
    auto &loginSprite = registry.get<Sprite>(loginButton);
    setSpriteSize(registry, loginButton, 2 * loginSprite.w / 3, 2 * loginSprite.h / 3);
    setPopupUIButtonSize(registry, loginButton, loginSprite.w, loginSprite.h);
    setPos(registry, loginButton, loginSprite.w / 8, loginSprite.h / 8);
    setBoundingBox(registry, loginButton, loginSprite.w, loginSprite.h);
    loginSprite.frame = 1;
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
    setSpriteSize(registry, achievementsButton, 4 * achievementsSprite.w / 5, 4 * achievementsSprite.h / 5);
    setPopupUIButtonSize(registry, achievementsButton, achievementsSprite.w, achievementsSprite.h);
    setPos(registry, achievementsButton, (panel.w / 3 - achievementsSprite.w) / 2, 3 * panel.h / 5 - achievementsSprite.h);
    setBoundingBox(registry, achievementsButton, achievementsSprite.w, achievementsSprite.h);

    auto creditsButton = createPopupUIButton(registry, parent, UIAction::CREDITS, 150);
    const auto &creditsSprite = registry.get<Sprite>(creditsButton);
    setSpriteSize(registry, creditsButton, 4 * creditsSprite.w / 5, 4 * creditsSprite.h / 5);
    setPopupUIButtonSize(registry, creditsButton, creditsSprite.w, creditsSprite.h);
    setPos(registry, creditsButton, (panel.w - creditsSprite.w) / 2, 3 * panel.h / 5 - creditsSprite.h / 2);
    setBoundingBox(registry, creditsButton, creditsSprite.w, creditsSprite.h);

    auto leaderboardButton = createPopupUIButton(registry, parent, UIAction::LEADERBOARD, 150);
    const auto &leaderboardSprite = registry.get<Sprite>(leaderboardButton);
    setSpriteSize(registry, leaderboardButton, 4 * leaderboardSprite.w / 5, 4 * leaderboardSprite.h / 5);
    setPopupUIButtonSize(registry, leaderboardButton, leaderboardSprite.w, leaderboardSprite.h);
    setPos(registry, leaderboardButton, (5 * panel.w / 3 - leaderboardSprite.w) / 2, 3 * panel.h / 5 - leaderboardSprite.h);
    setBoundingBox(registry, leaderboardButton, leaderboardSprite.w, leaderboardSprite.h);
}


void createPlayPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::PLAY_GAME, logicalWidth, 0, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_6_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_6_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_6_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_6_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto theGameButton = createPopupUIButton(registry, parent, UIAction::THE_GAME, 20);
    auto &theGameSprite = registry.get<Sprite>(theGameButton);
    setSpriteSize(registry, theGameButton, 4 * theGameSprite.w / 5, 4 * theGameSprite.h / 5);
    setPopupUIButtonSize(registry, theGameButton, theGameSprite.w, theGameSprite.h);
    setPos(registry, theGameButton, (panel.w - theGameSprite.w) / 2, panel.h / 2 - 2 * theGameSprite.h / 3);
    setBoundingBox(registry, theGameButton, theGameSprite.w, theGameSprite.h);
    registry.assign<PulseAnimation>(theGameButton, 0.f, .5f, .1f, 5000_ui32);

    auto theGameStrHandle = textureCache.handle("str/game/the_game");
    auto theGameStrEntity = createSprite(registry, parent, theGameStrHandle, 20);
    setPos(registry, theGameStrEntity, panel.w / 2 - theGameStrHandle->width() / 2, panel.h / 2 + (theGameSprite.h + theGameStrHandle->height()) / 3);

    auto endlessStrHandle = textureCache.handle("str/game/endless");
    auto endlessStrEntity = createSprite(registry, parent, endlessStrHandle, 20);
    setPos(registry, endlessStrEntity, panel.w / 5 - endlessStrHandle->width() / 2, panel.h / 2 + (theGameSprite.h + endlessStrHandle->height()) / 3);

    auto tetrisStrHandle = textureCache.handle("str/game/tetris");
    auto tetrisStrEntity = createSprite(registry, parent, tetrisStrHandle, 20);
    setPos(registry, tetrisStrEntity, 4 * panel.w / 5 - tetrisStrHandle->width() / 2, panel.h / 2 + (theGameSprite.h + tetrisStrHandle->height()) / 3);
}


void refreshPlayPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    entity_type parent{};

    for(auto entity: registry.view<Panel>()) {
        if(registry.get<Panel>(entity).type == PanelType::PLAY_GAME) {
            parent = entity;
            break;
        }
    }

    const auto &panel = registry.get<Panel>(parent);

    auto addOtherGamesButtons = [&](auto endlessAction, auto tetrisAction) {
        auto endlessButton = createPopupUIButton(registry, parent, endlessAction, 20);
        auto &endlessSprite = registry.get<Sprite>(endlessButton);
        setSpriteSize(registry, endlessButton, 2 * endlessSprite.w / 3, 2 * endlessSprite.h / 3);
        setPopupUIButtonSize(registry, endlessButton, endlessSprite.w, endlessSprite.h);
        setPos(registry, endlessButton, panel.w / 5 - endlessSprite.w / 2, panel.h / 2 - 2 * endlessSprite.h / 3);
        setBoundingBox(registry, endlessButton, endlessSprite.w, endlessSprite.h);
        registry.assign<ExpiringContent>(endlessButton);

        auto tetrisButton = createPopupUIButton(registry, parent, tetrisAction, 20);
        auto &tetrisSprite = registry.get<Sprite>(tetrisButton);
        setSpriteSize(registry, tetrisButton, 2 * tetrisSprite.w / 3, 2 * tetrisSprite.h / 3);
        setPopupUIButtonSize(registry, tetrisButton, tetrisSprite.w, tetrisSprite.h);
        setPos(registry, tetrisButton, 4 * panel.w / 5 - tetrisSprite.w / 2, panel.h / 2 - 2 * tetrisSprite.h / 3);
        setBoundingBox(registry, tetrisButton, tetrisSprite.w, tetrisSprite.h);
        registry.assign<ExpiringContent>(tetrisButton);
    };

    const bool unlocked = registry.has<FaceSmashSupporter>()
            || registry.has<MoreGamesUnlocked>();

    // we have still to develop them after all :-)
    if(false && unlocked) {
        // if unlocked, give access to all the other games ...
        addOtherGamesButtons(UIAction::ENDLESS, UIAction::TETRIS);
    } else {
        // ... otherwise the other games around are locked
        addOtherGamesButtons(UIAction::LOCKED, UIAction::LOCKED);

        auto unlock1StrHandle = textureCache.handle("str/howto/unlock/1");
        auto unlock1StrEntity = createSprite(registry, parent, unlock1StrHandle, 20);
        setPos(registry, unlock1StrEntity, (panel.w - unlock1StrHandle->width()) / 2, unlock1StrHandle->height() / 2);
        registry.assign<FadeAnimation>(unlock1StrEntity, 255, 0, 3000_ui32, 0_ui32, &easeInExpo);
        registry.assign<ExpiringContent>(unlock1StrEntity);

        auto unlock2StrHandle = textureCache.handle("str/howto/unlock/2");
        auto unlock2StrEntity = createSprite(registry, parent, unlock2StrHandle, 20);
        setPos(registry, unlock2StrEntity, (panel.w - unlock2StrHandle->width()) / 2, panel.h - 3 * unlock2StrHandle->height() / 2);
        registry.assign<FadeAnimation>(unlock2StrEntity, 255, 0, 3000_ui32, 0_ui32, &easeInExpo);
        registry.assign<ExpiringContent>(unlock2StrEntity);
    }
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

    auto offset = registry.get<Transform>(titleEntity).y + 5 * titleHandle->height() / 3;

    auto thanksHandle = textureCache.handle("str/credits/thanks");
    auto thanksEntity = createSprite(registry, parent, thanksHandle, 20);
    setPos(registry, thanksEntity, (panel.w - thanksHandle->width()) / 2, offset);
    offset += 9 * thanksHandle->height() / 8;

    auto playingHandle = textureCache.handle("str/credits/playing");
    auto playingEntity = createSprite(registry, parent, playingHandle, 20);
    setPos(registry, playingEntity, (panel.w - playingHandle->width()) / 2, offset);
    offset += 5 * playingHandle->height() / 4;

    auto facesmashHandle = textureCache.handle("str/credits/facesmash");
    auto facesmashEntity = createSprite(registry, parent, facesmashHandle, 20);
    setPos(registry, facesmashEntity, (panel.w - facesmashHandle->width()) / 2, offset);
    offset += 4 * facesmashHandle->height();

    auto wearegameeHandle = textureCache.handle("str/credits/wearegamee");
    auto wearegameeEntity = createSprite(registry, parent, wearegameeHandle, 20);
    setPos(registry, wearegameeEntity, (panel.w - wearegameeHandle->width()) / 2, offset);
    offset += 9 * wearegameeHandle->height() / 8;

    auto authorsHandle = textureCache.handle("str/credits/authors");
    auto authorsEntity = createSprite(registry, parent, authorsHandle, 20);
    setPos(registry, authorsEntity, (panel.w - authorsHandle->width()) / 2, offset);
    offset += 5 * authorsHandle->height();

    auto oslicensesHandle = textureCache.handle("str/credits/oslicenses");
    auto oslicensesEntity = createSprite(registry, parent, oslicensesHandle, 20);
    setPos(registry, oslicensesEntity, (panel.w - oslicensesHandle->width()) / 2, offset);
    offset += 5 * oslicensesHandle->height() / 4;

    auto gpgLicenseButton = createPopupUIButton(registry, parent, UIAction::GPG_LICENSE, 20);
    const auto &gpgLicenseSprite = registry.get<Sprite>(gpgLicenseButton);
    setSpriteSize(registry, gpgLicenseButton, gpgLicenseSprite.w / 2, gpgLicenseSprite.h / 2);
    setPopupUIButtonSize(registry, gpgLicenseButton, gpgLicenseSprite.w, gpgLicenseSprite.h);
    setBoundingBox(registry, gpgLicenseButton, gpgLicenseSprite.w, gpgLicenseSprite.h);
    setPos(registry, gpgLicenseButton, (panel.w - gpgLicenseSprite.w) / 2, offset);
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
    auto &textureCache = Locator::TextureCache::ref();

    entity_type parent{};

    for(auto entity: registry.view<Panel>()) {
        if(registry.get<Panel>(entity).type == PanelType::SUPPORT) {
            parent = entity;
            break;
        }
    }

    const auto &panel = registry.get<Panel>(parent);

    if(registry.has<FaceSmashSupporter>()) {
        auto thankHandle = textureCache.handle("str/support/thank");
        auto thankEntity = createSprite(registry, parent, thankHandle, 20);
        setPos(registry, thankEntity, (panel.w - thankHandle->width()) / 2, 1 * panel.h / 5);
        registry.assign<ExpiringContent>(thankEntity);

        auto youHandle = textureCache.handle("str/support/you");
        auto youEntity = createSprite(registry, parent, youHandle, 20);
        setPos(registry, youEntity, (panel.w - youHandle->width()) / 2, 3 * panel.h / 10);
        registry.assign<ExpiringContent>(youEntity);

        auto beingHandle = textureCache.handle("str/support/being");
        auto beingEntity = createSprite(registry, parent, beingHandle, 20);
        setPos(registry, beingEntity, (panel.w - beingHandle->width()) / 2, 3 * panel.h / 6);
        registry.assign<ExpiringContent>(beingEntity);

        auto facesmashHandle = textureCache.handle("str/support/facesmash");
        auto facesmashEntity = createSprite(registry, parent, facesmashHandle, 20);
        setPos(registry, facesmashEntity, (panel.w - facesmashHandle->width()) / 2, 4 * panel.h / 6);
        registry.assign<ExpiringContent>(facesmashEntity);

        auto supporterHandle = textureCache.handle("str/support/supporter");
        auto supporterEntity = createSprite(registry, parent, supporterHandle, 20);
        setPos(registry, supporterEntity, (panel.w - supporterHandle->width()) / 2, 8 * panel.h / 11);
        registry.assign<ExpiringContent>(supporterEntity);
    } else {
        auto makingHandle = textureCache.handle("str/support/making");
        auto makingEntity = createSprite(registry, parent, makingHandle, 20);
        setPos(registry, makingEntity, (panel.w - makingHandle->width()) / 2, 1 * panel.h / 6);
        registry.assign<ExpiringContent>(makingEntity);

        auto timeHandle = textureCache.handle("str/support/time");
        auto timeEntity = createSprite(registry, parent, timeHandle, 20);
        setPos(registry, timeEntity, (panel.w - timeHandle->width()) / 2, 2 * panel.h / 9);
        registry.assign<ExpiringContent>(timeEntity);

        auto lotHandle = textureCache.handle("str/support/lot");
        auto lotEntity = createSprite(registry, parent, lotHandle, 20);
        setPos(registry, lotEntity, (panel.w - lotHandle->width()) / 2, 3 * panel.h / 9);
        registry.assign<ExpiringContent>(lotEntity);

        auto coffeeHandle = textureCache.handle("str/support/coffee");
        auto coffeeEntity = createSprite(registry, parent, coffeeHandle, 20);
        setPos(registry, coffeeEntity, (panel.w - coffeeHandle->width()) / 2, 4 * panel.h / 10);
        registry.assign<ExpiringContent>(coffeeEntity);

        auto shopButton = createPopupUIButton(registry, parent, UIAction::SHOP, 20);
        const auto &shopTransform = registry.get<Transform>(shopButton);
        const auto &shopSprite = registry.get<Sprite>(shopButton);
        setSpriteSize(registry, shopButton, 4 * shopSprite.w / 5, 4 * shopSprite.h / 5);
        setPopupUIButtonSize(registry, shopButton, shopSprite.w, shopSprite.h);
        setPos(registry, shopButton, (panel.w - shopSprite.w) / 2, 5 * panel.h / 9);
        setBoundingBox(registry, shopButton, 3 * shopSprite.w / 2, 3 * shopSprite.h / 2);
        registry.assign<PulseAnimation>(shopButton, 0.f, .8f, .2f, 3000_ui32);
        registry.get<Sprite>(shopButton).frame = 0;
        registry.assign<ExpiringContent>(shopButton);

        auto removeHandle = textureCache.handle("str/support/remove");
        auto removeEntity = createSprite(registry, parent, removeHandle, 20);
        setPos(registry, removeEntity, shopTransform.x - removeHandle->width() - shopSprite.w / 4, shopTransform.y + shopSprite.h / 2 - 3 * removeHandle->height() / 2);
        registry.assign<ExpiringContent>(removeEntity);

        auto adsHandle = textureCache.handle("str/support/ads");
        auto adsEntity = createSprite(registry, parent, adsHandle, 20);
        setPos(registry, adsEntity, shopTransform.x - adsHandle->width() - shopSprite.w / 4, shopTransform.y + shopSprite.h / 2 + adsHandle->height() / 2);
        registry.assign<ExpiringContent>(adsEntity);

        auto unlockHandle = textureCache.handle("str/support/unlock");
        auto unlockEntity = createSprite(registry, parent, unlockHandle, 20);
        setPos(registry, unlockEntity, shopTransform.x + 5 * shopSprite.w / 4, shopTransform.y + shopSprite.h / 2 - 3 * unlockHandle->height() / 2);
        registry.assign<ExpiringContent>(unlockEntity);

        auto gamesHandle = textureCache.handle("str/support/games");
        auto gamesEntity = createSprite(registry, parent, gamesHandle, 20);
        setPos(registry, gamesEntity, shopTransform.x + 5 * shopSprite.w / 4, shopTransform.y + shopSprite.h / 2 + gamesHandle->height() / 2);
        registry.assign<ExpiringContent>(gamesEntity);

        auto offerHandle = textureCache.handle("str/support/offer");
        auto offerEntity = createSprite(registry, parent, offerHandle, 20);
        setPos(registry, offerEntity, (panel.w - offerHandle->width()) / 2, 7 * panel.h / 9);
        registry.assign<ExpiringContent>(offerEntity);

        auto nextHandle = textureCache.handle("str/support/next");
        auto nextEntity = createSprite(registry, parent, nextHandle, 20);
        setPos(registry, nextEntity, (panel.w - nextHandle->width()) / 2, 8 * panel.h / 9);
        registry.assign<ExpiringContent>(nextEntity);
    }
}


void createSettingsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::SETTINGS, logicalWidth, 0, logicalWidth, logicalHeight / 4);
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
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 4);

    auto &settings = gamee::Locator::Settings::ref();

    const bool audio = settings.read("audio/available", true);
    const bool video = settings.read("video/available", true);
    const bool haptic = settings.read("haptic/available", true);

    auto audioButton = createPopupUIButton(registry, parent, UIAction::SWITCH_AUDIO, 20);
    auto &audioSprite = registry.get<Sprite>(audioButton);
    setSpriteSize(registry, audioButton, 2 * audioSprite.w / 3, 2 * audioSprite.h / 3);
    setPopupUIButtonSize(registry, audioButton, audioSprite.w, audioSprite.h);
    setPos(registry, audioButton, panel.w / 4 - audioSprite.w / 2, panel.h / 2 - audioSprite.h / 3);
    setBoundingBox(registry, audioButton, audioSprite.w, audioSprite.h);
    audioSprite.frame = audio ? 0 : 1;

    auto videoButton = createPopupUIButton(registry, parent, UIAction::SWITCH_VIDEO, 20);
    auto &videoSprite = registry.get<Sprite>(videoButton);
    setSpriteSize(registry, videoButton, 2 * videoSprite.w / 3, 2 * videoSprite.h / 3);
    setPopupUIButtonSize(registry, videoButton, videoSprite.w, videoSprite.h);
    setPos(registry, videoButton, panel.w / 2 - videoSprite.w / 2, panel.h / 2 - videoSprite.h / 3);
    setBoundingBox(registry, videoButton, videoSprite.w, videoSprite.h);
    videoSprite.frame = video ? 0 : 1;

    auto hapticButton = createPopupUIButton(registry, parent, UIAction::SWITCH_HAPTIC, 20);
    auto &hapticSprite = registry.get<Sprite>(hapticButton);
    setSpriteSize(registry, hapticButton, 2 * hapticSprite.w / 3, 2 * hapticSprite.h / 3);
    setPopupUIButtonSize(registry, hapticButton, hapticSprite.w, hapticSprite.h);
    setPos(registry, hapticButton, 3 * panel.w / 4 - hapticSprite.w / 2, panel.h / 2 - hapticSprite.h / 3);
    setBoundingBox(registry, hapticButton, hapticSprite.w, hapticSprite.h);
    hapticSprite.frame = haptic ? 0 : 1;

    auto audioStrHandle = textureCache.handle("str/audio");
    auto audioStrEntity = createSprite(registry, parent, audioStrHandle, 20);
    setPos(registry, audioStrEntity, panel.w / 4 - audioStrHandle->width() / 2, panel.h - 2 * audioStrHandle->height());

    auto videoStrHandle = textureCache.handle("str/video");
    auto videoStrEntity = createSprite(registry, parent, videoStrHandle, 20);
    setPos(registry, videoStrEntity, panel.w / 2 - videoStrHandle->width() / 2, panel.h - 2 * videoStrHandle->height());

    auto hapticStrHandle = textureCache.handle("str/haptic");
    auto hapticStrEntity = createSprite(registry, parent, hapticStrHandle, 20);
    setPos(registry, hapticStrEntity, 3 * panel.w / 4 - hapticStrHandle->width() / 2, panel.h - 2 * hapticStrHandle->height());
}


void createTutorialTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TUTORIAL_TOP, 0, -logicalHeight / 2, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto addButton = [&](FaceType type, int idx) {
        auto entity = createFaceBlueprint(registry, type, 150);
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
        auto entity = createItemBlueprint(registry, type, 150);
        const auto &sprite = registry.get<Sprite>(entity);

        const auto width = 3 * sprite.w / 5;
        const auto height = 3 * sprite.h / 5;

        const auto buttonOffset = (panel.w - ((numberOfItems - 1) * width + (numberOfItems - 2) * 10)) / 2;

        setPos(registry, entity, buttonOffset + idx * (width + 10), height / 2);
        setSpriteSize(registry, entity, width, height);
        registry.get<Transform>(entity).parent = parent;
    };

    addButton(ItemType::SPEED_UP, 0);
    addButton(ItemType::SLOW_DOWN, 1);
    addButton(ItemType::FOUNTAIN, 2);
    addButton(ItemType::BOMB, 3);
    addButton(ItemType::MONEY, 4);

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
    setPos(registry, playerScoreEntity, offset + .1f * scoreHandle->width(), .4f * scoreHandle->height());
    offset = registry.get<Transform>(playerScoreEntity).x + 1.1f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(PlayerScoreObserver::entities)>::value; ++i) {
        playerScoreObserver.entities[i] = createHUD(registry, parent, sym0Handle, 160);
        registry.get<HUD>(playerScoreObserver.entities[i]).handle = i ? symEmptyHandle : sym0Handle;
        setPos(registry, playerScoreObserver.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }

    offset = panel.w - .2f * scoreHandle->width() - 2 * sym0Handle->width() - timerHandle->width();

    auto timerEntity = createHUD(registry, parent, timerHandle, 160);
    auto &timerObserver = registry.assign<TimerObserver>(timerEntity);
    setPos(registry, timerEntity, offset, .4f * scoreHandle->height());
    offset = registry.get<Transform>(timerEntity).x + timerHandle->width() + .1f * scoreHandle->width();

    for(auto i = 0u; i < std::extent<decltype(TimerObserver::entities)>::value; ++i) {
        timerObserver.entities[i] = createHUD(registry, parent, sym0Handle, 160);
        setPos(registry, timerObserver.entities[i], offset, .4f * scoreHandle->height());
        offset += sym0Handle->width();
    }
}


void createGameOverPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::GAME_OVER, logicalWidth, 0, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/gameover");
    auto titleEntity = createSprite(registry, parent, titleHandle, 150);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, (panel.h / 2 - titleHandle->height()) / 2);

    auto menuButton = createPopupUIButton(registry, parent, UIAction::MENU, 150);
    auto &menuSprite = registry.get<Sprite>(menuButton);
    setSpriteSize(registry, menuButton, 4 * menuSprite.w / 5, 4 * menuSprite.h / 5);
    setPopupUIButtonSize(registry, menuButton, menuSprite.w, menuSprite.h);
    setPos(registry, menuButton, (panel.w / 3 - menuSprite.w) / 2, (3 * panel.h / 2 - menuSprite.h) / 2);
    setBoundingBox(registry, menuButton, menuSprite.w, menuSprite.h);

    auto restartButton = createPopupUIButton(registry, parent, UIAction::RESTART, 150);
    const auto &restartSprite = registry.get<Sprite>(restartButton);
    setPos(registry, restartButton, (panel.w - restartSprite.w) / 2, (3 * panel.h / 2 - restartSprite.h) / 2);
    registry.assign<PulseAnimation>(restartButton, 10.f, .5f, .08f, 3000_ui32);
}


void refreshGameOverPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto &recorder = Locator::AvRecorder::ref();

    entity_type parent{};

    for(auto entity: registry.view<Panel>()) {
        if(registry.get<Panel>(entity).type == PanelType::GAME_OVER) {
            parent = entity;
            break;
        }
    }

    const auto &panel = registry.get<Panel>(parent);

    if(registry.has<PlayerScore>()) {
        const auto &playerScore = registry.get<PlayerScore>();

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

            auto sym0Handle = toStrSmallHandle(0);

            auto res0Entity = createSprite(registry, parent, sym0Handle, 150);
            registry.get<Sprite>(res0Entity).handle = toStrSmallHandle((playerScore.*member) / 10);
            setPos(registry, res0Entity, xOffset, yOffset);
            registry.assign<ExpiringContent>(res0Entity);
            registry.assign<BoundingBox>(res0Entity, sym0Handle->width(), sym0Handle->height());
            xOffset += sym0Handle->width();

            auto res1Entity = createSprite(registry, parent, sym0Handle, 150);
            registry.get<Sprite>(res1Entity).handle = toStrSmallHandle((playerScore.*member) % 10);
            setPos(registry, res1Entity, xOffset, yOffset);
            registry.assign<ExpiringContent>(res1Entity);
            registry.assign<BoundingBox>(res1Entity, sym0Handle->width(), sym0Handle->height());
            xOffset += sym0Handle->width();
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

        auto sym0Handle = toStrNormalHandle(0);

        auto printScore = [&](int score, auto x) {
            auto entity = createSprite(registry, parent, sym0Handle, 150);
            registry.get<Sprite>(entity).handle = toStrNormalHandle(score);
            setPos(registry, entity, panel.w / 2 + x, panel.h / 2 + 2 * sym0Handle->height());
            registry.assign<BoundingBox>(entity, sym0Handle->width(), sym0Handle->height());
            registry.assign<ExpiringContent>(entity);
        };

        auto score = playerScore.score;
        printScore(score % 10, 3 * sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, -sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, -3 * sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, -5 * sym0Handle->width() / 2);
    }

    auto shareButton = createPopupUIButton(registry, parent, UIAction::SHARE, 150);
    const auto &shareSprite = registry.get<Sprite>(shareButton);
    setSpriteSize(registry, shareButton, 4 * shareSprite.w / 5, 4 * shareSprite.h / 5);
    setPopupUIButtonSize(registry, shareButton, shareSprite.w, shareSprite.h);
    setPos(registry, shareButton, (5 * panel.w / 3 - shareSprite.w) / 2, (3 * panel.h / 2 - shareSprite.h) / 2);
    setBoundingBox(registry, shareButton, shareSprite.w, shareSprite.h);
    registry.assign<ExpiringContent>(shareButton);

    if(recorder.supportExport()) {
        registry.get<Sprite>(shareButton).frame = 1;
    } else {
        registry.get<UIButton>(shareButton).enabled = false;
        registry.get<Sprite>(shareButton).frame = 0;
    }
}


void createTrainingRightPanel(Registry &registry) {
    auto parent = createPanel(registry, PanelType::TRAINING_RIGHT, logicalWidth, 0, logicalWidth/8, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    const auto progressBarEntity = createVerticalProgressBar(registry, parent, 160);
    const auto &progressBarSprite = registry.get<Sprite>(progressBarEntity);
    setPos(registry, progressBarEntity, (panel.w - progressBarSprite.w) / 2, (panel.h - progressBarSprite.h) / 2);
}


void createTrainingTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::TRAINING_TOP, 0, -logicalHeight / 4, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto titleHandle = textureCache.handle("str/training");
    auto titleEntity = createSprite(registry, parent, titleHandle, 160);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);
}


void createTrainingBottomPanel(Registry &registry) {
    auto parent = createPanel(registry, PanelType::TRAINING_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto createButton = [&](auto type, auto pos) {
        auto entity = createFaceButton(registry, type, 160);

        const auto &sprite = registry.get<Sprite>(entity);
        const auto w = 2 * sprite.w / 3;
        const auto h = 2 * sprite.h / 3;

        const auto x = (panel.w - 23 * w / 3) / 2 + pos * 4 * w / 3;

        setPos(registry, entity, x, (panel.h - h) / 2);
        setBoundingBox(registry, entity, w, h);
        setSpriteSize(registry, entity, w, h);
        setFaceButtonSize(registry, entity, w, h);
        registry.get<Transform>(entity).parent = parent;
        registry.assign<PulseAnimation>(entity, 0.f, .6f, .06f, 2000_ui32);
    };

    createButton(FaceType::ANGRY, 0);
    createButton(FaceType::DISGUSTED, 1);
    createButton(FaceType::FEARFUL, 2);
    createButton(FaceType::HAPPY, 3);
    createButton(FaceType::SAD, 4);
    createButton(FaceType::SURPRISED, 5);
}


void createCamera(Registry &registry) {
    auto camera = registry.create();
    registry.assign<Transform>(camera, camera, 0.f, 0.f);
    registry.assign<Camera>(entt::tag_t{}, camera);
}


void createCameraFrame(Registry &registry) {
    auto handle = Locator::TextureCache::ref().handle("camera/frame");
    auto &cameraService = Locator::Camera::ref();

    auto frame = registry.create();
    registry.assign<CameraFrame>(entt::tag_t{}, frame);

    int width = 0;
    int height = 0;
    float angle = 0.f;

    if(cameraService.height() > cameraService.width()) {
        width = handle->width() * logicalHeight / handle->height();
        height = logicalHeight;

        // it shouldn't happen but who knows? just in case... :-)
        if(width < logicalWidth) {
            height = height * logicalWidth / width;
            width = logicalWidth;
        }
    } else {
        width = logicalHeight;
        height = handle->height() * logicalHeight / handle->width();

        // it shouldn't happen but who knows? just in case... :-)
        if(height < logicalWidth) {
            width = width * logicalWidth / height;
            height = logicalWidth;
        }

        angle = 90.f;
    }

    registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), width, height);
    registry.assign<Transform>(frame, frame, (logicalWidth - width) / 2.f, (logicalHeight - height) / 2.f);
    registry.assign<Renderable>(frame, angle, 90, 0, true);
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

    auto &debug = registry.assign<DebugInfo>(entt::tag_t{}, registry.create());

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
