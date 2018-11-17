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

    auto multiplayerButton = createPopupUIButton(registry, parent, UIAction::MULTIPLAYER, 150);
    auto &multiplayerSprite = registry.get<Sprite>(multiplayerButton);
    setSpriteSize(registry, multiplayerButton, 4 * multiplayerSprite.w / 5, 4 * multiplayerSprite.h / 5);
    setPopupUIButtonSize(registry, multiplayerButton, multiplayerSprite.w, multiplayerSprite.h);
    setPos(registry, multiplayerButton, (panel.w / 3 - multiplayerSprite.w) / 2, 2 * panel.h / 5);
    setBoundingBox(registry, multiplayerButton, multiplayerSprite.w, multiplayerSprite.h);

    auto multiplayerLabelHandle = textureCache.handle("str/menu/multiplayer");
    auto multiplayerLabelEntity = createSprite(registry, parent, multiplayerLabelHandle, 150);
    setPos(registry, multiplayerLabelEntity, (panel.w / 3 - multiplayerLabelHandle->width()) / 2, 2 * panel.h / 5 + multiplayerSprite.h + multiplayerLabelHandle->height() / 2);

    auto playButton = createPopupUIButton(registry, parent, UIAction::THE_GAME, 150);
    const auto &playSprite = registry.get<Sprite>(playButton);
    setPos(registry, playButton, (panel.w - playSprite.w) / 2, 2 * panel.h / 5 - playSprite.h / 2);
    registry.assign<PulseAnimation>(playButton, 10.f, .4f, .1f, 3000_ui32);

    auto playLabelHandle = textureCache.handle("str/menu/play");
    auto playLabelEntity = createSprite(registry, parent, playLabelHandle, 150);
    setPos(registry, playLabelEntity, (panel.w - playLabelHandle->width()) / 2, 2 * panel.h / 5 + playSprite.h / 2 + playLabelHandle->height() / 2);

    auto trainingButton = createPopupUIButton(registry, parent, UIAction::TRAINING, 150);
    const auto &trainingSprite = registry.get<Sprite>(trainingButton);
    setSpriteSize(registry, trainingButton, 4 * trainingSprite.w / 5, 4 * trainingSprite.h / 5);
    setPopupUIButtonSize(registry, trainingButton, trainingSprite.w, trainingSprite.h);
    setPos(registry, trainingButton, (5 * panel.w / 3 - trainingSprite.w) / 2, 2 * panel.h / 5);
    setBoundingBox(registry, trainingButton, trainingSprite.w, trainingSprite.h);

    auto trainingLabelHandle = textureCache.handle("str/menu/training");
    auto trainingLabelEntity = createSprite(registry, parent, trainingLabelHandle, 150);
    setPos(registry, trainingLabelEntity, (5 * panel.w / 3 - trainingLabelHandle->width()) / 2, 2 * panel.h / 5 + trainingSprite.h + trainingLabelHandle->height() / 2);

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

    auto achievementsLabelHandle = textureCache.handle("str/menu/achievements");
    auto achievementsLabelEntity = createSprite(registry, parent, achievementsLabelHandle, 150);
    setPos(registry, achievementsLabelEntity, (panel.w / 3 - achievementsLabelHandle->width()) / 2, 3 * panel.h / 5 - achievementsSprite.h - 3 * achievementsLabelHandle->height() / 2);

    auto infoButton = createPopupUIButton(registry, parent, UIAction::INFO, 150);
    const auto &infoSprite = registry.get<Sprite>(infoButton);
    setSpriteSize(registry, infoButton, 4 * infoSprite.w / 5, 4 * infoSprite.h / 5);
    setPopupUIButtonSize(registry, infoButton, infoSprite.w, infoSprite.h);
    setPos(registry, infoButton, (panel.w - infoSprite.w) / 2, 3 * panel.h / 5 - infoSprite.h / 2);
    setBoundingBox(registry, infoButton, infoSprite.w, infoSprite.h);

    auto infoLabelHandle = textureCache.handle("str/menu/info");
    auto infoLabelEntity = createSprite(registry, parent, infoLabelHandle, 150);
    setPos(registry, infoLabelEntity, (panel.w - infoLabelHandle->width()) / 2, 3 * panel.h / 5 - infoSprite.h / 2 - 3 * infoLabelHandle->height() / 2);

    auto leaderboardButton = createPopupUIButton(registry, parent, UIAction::LEADERBOARD, 150);
    const auto &leaderboardSprite = registry.get<Sprite>(leaderboardButton);
    setSpriteSize(registry, leaderboardButton, 4 * leaderboardSprite.w / 5, 4 * leaderboardSprite.h / 5);
    setPopupUIButtonSize(registry, leaderboardButton, leaderboardSprite.w, leaderboardSprite.h);
    setPos(registry, leaderboardButton, (5 * panel.w / 3 - leaderboardSprite.w) / 2, 3 * panel.h / 5 - leaderboardSprite.h);
    setBoundingBox(registry, leaderboardButton, leaderboardSprite.w, leaderboardSprite.h);

    auto leaderboardLabelHandle = textureCache.handle("str/menu/leaderboard");
    auto leaderboardLabelEntity = createSprite(registry, parent, leaderboardLabelHandle, 150);
    setPos(registry, leaderboardLabelEntity, (5 * panel.w / 3 - leaderboardLabelHandle->width()) / 2, 3 * panel.h / 5 - leaderboardSprite.h - 3 * leaderboardLabelHandle->height() / 2);
}


void createInfoPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::INFO, logicalWidth, 0, logicalWidth, 3 * logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_3_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_3_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_3_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_3_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto titleHandle = textureCache.handle("str/info");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 2);

    auto offset = registry.get<Transform>(titleEntity).y + 4 * titleHandle->height() / 3;

    auto thanksHandle = textureCache.handle("str/info/thanks");
    auto thanksEntity = createSprite(registry, parent, thanksHandle, 20);
    setPos(registry, thanksEntity, (panel.w - thanksHandle->width()) / 2, offset);
    offset += 9 * thanksHandle->height() / 8;

    auto playingHandle = textureCache.handle("str/info/playing");
    auto playingEntity = createSprite(registry, parent, playingHandle, 20);
    setPos(registry, playingEntity, (panel.w - playingHandle->width()) / 2, offset);
    offset += 5 * playingHandle->height() / 4;

    auto facesmashHandle = textureCache.handle("str/info/facesmash");
    auto facesmashEntity = createSprite(registry, parent, facesmashHandle, 20);
    setPos(registry, facesmashEntity, (panel.w - facesmashHandle->width()) / 2, offset);
    offset += 3 * facesmashHandle->height() / 2;

    auto wearegameeHandle = textureCache.handle("str/info/wearegamee");
    auto wearegameeEntity = createSprite(registry, parent, wearegameeHandle, 20);
    setPos(registry, wearegameeEntity, (panel.w - wearegameeHandle->width()) / 2, offset);
    offset += 9 * wearegameeHandle->height() / 8;

    auto authorsHandle = textureCache.handle("str/info/authors");
    auto authorsEntity = createSprite(registry, parent, authorsHandle, 20);
    setPos(registry, authorsEntity, (panel.w - authorsHandle->width()) / 2, offset);
    offset += 2 * authorsHandle->height();

    auto gpgLicenseButton = createPopupUIButton(registry, parent, UIAction::GPG_LICENSE, 20);
    const auto &gpgLicenseTransform = registry.get<Transform>(gpgLicenseButton);
    const auto &gpgLicenseSprite = registry.get<Sprite>(gpgLicenseButton);
    setSpriteSize(registry, gpgLicenseButton, gpgLicenseSprite.w / 2, gpgLicenseSprite.h / 2);
    setPopupUIButtonSize(registry, gpgLicenseButton, gpgLicenseSprite.w, gpgLicenseSprite.h);
    setBoundingBox(registry, gpgLicenseButton, gpgLicenseSprite.w, gpgLicenseSprite.h);
    setPos(registry, gpgLicenseButton, gpgLicenseSprite.w / 3, panel.h - 3 * gpgLicenseSprite.h / 2);

    auto oslicensesHandle = textureCache.handle("str/info/oslicenses");
    auto oslicensesEntity = createSprite(registry, parent, oslicensesHandle, 20);
    setPos(registry, oslicensesEntity, gpgLicenseTransform.x + 5 * gpgLicenseSprite.w / 4, gpgLicenseTransform.y + (gpgLicenseSprite.h - oslicensesHandle->height()) / 2);
}


void refreshInfoPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    entity_type parent{};

    for(auto entity: registry.view<Panel>()) {
        if(registry.get<Panel>(entity).type == PanelType::INFO) {
            parent = entity;
            break;
        }
    }

    const auto &panel = registry.get<Panel>(parent);
    auto offset = panel.h / 2;

    if(registry.has<FaceSmashSupporter>()) {
        auto welcomeHandle = textureCache.handle("str/info/welcome");
        auto welcomeEntity = createSprite(registry, parent, welcomeHandle, 20);
        setPos(registry, welcomeEntity, (panel.w - welcomeHandle->width()) / 2, offset);
        registry.assign<ExpiringContent>(welcomeEntity);
        offset += 9 * welcomeHandle->height() / 8;

        auto facesmashHandle = textureCache.handle("str/info/facesmash");
        auto facesmashEntity = createSprite(registry, parent, facesmashHandle, 20);
        setPos(registry, facesmashEntity, (panel.w - facesmashHandle->width()) / 2, offset);
        registry.assign<ExpiringContent>(facesmashEntity);
        offset += 9 * facesmashHandle->height() / 8;

        auto supporterHandle = textureCache.handle("str/info/supporter");
        auto supporterEntity = createSprite(registry, parent, supporterHandle, 20);
        setPos(registry, supporterEntity, (panel.w - supporterHandle->width()) / 2, offset);
        registry.assign<ExpiringContent>(supporterEntity);
        offset += 7 * facesmashHandle->height() / 4;

        auto addFace = [&](FaceType type, int idx) {
            auto entity = createFaceBlueprint(registry, type, 20);
            const auto &sprite = registry.get<Sprite>(entity);

            const auto width = 3 * sprite.w / 5;
            const auto height = 3 * sprite.h / 5;

            const auto buttonOffset = (panel.w - (numberOfFaces * width + (numberOfFaces - 1) * 10)) / 2;

            setPos(registry, entity, buttonOffset + idx * (width + 10), offset);
            setSpriteSize(registry, entity, width, height);
            registry.get<Transform>(entity).parent = parent;
        };

        addFace(FaceType::ANGRY, 0);
        addFace(FaceType::DISGUSTED, 1);
        addFace(FaceType::HAPPY, 2);
        addFace(FaceType::SURPRISED, 3);
        addFace(FaceType::FEARFUL, 4);
        addFace(FaceType::SAD, 5);
    } else {
        auto supportHandle = textureCache.handle("str/info/support");
        auto supportEntity = createSprite(registry, parent, supportHandle, 20);
        setPos(registry, supportEntity, (panel.w - supportHandle->width()) / 2, offset);
        registry.assign<ExpiringContent>(supportEntity);
        offset += 9 * supportHandle->height() / 8;

        auto buyHandle = textureCache.handle("str/info/buy");
        auto buyEntity = createSprite(registry, parent, buyHandle, 20);
        setPos(registry, buyEntity, (panel.w - buyHandle->width()) / 2, offset);
        registry.assign<ExpiringContent>(buyEntity);
        offset += 9 * buyHandle->height() / 8;

        auto offerHandle = textureCache.handle("str/info/offer");
        auto offerEntity = createSprite(registry, parent, offerHandle, 20);
        setPos(registry, offerEntity, (panel.w - offerHandle->width()) / 2, offset);
        registry.assign<ExpiringContent>(offerEntity);
        offset += 2 * buyHandle->height();

        auto shopButton = createPopupUIButton(registry, parent, UIAction::SHOP, 20);
        const auto &shopSprite = registry.get<Sprite>(shopButton);
        setSpriteSize(registry, shopButton, 4 * shopSprite.w / 5, 4 * shopSprite.h / 5);
        setPopupUIButtonSize(registry, shopButton, shopSprite.w, shopSprite.h);
        setPos(registry, shopButton, (panel.w - shopSprite.w) / 2, offset);
        setBoundingBox(registry, shopButton, 3 * shopSprite.w / 2, 3 * shopSprite.h / 2);
        registry.assign<PulseAnimation>(shopButton, 0.f, .8f, .2f, 3000_ui32);
        registry.get<Sprite>(shopButton).frame = 0;
        registry.assign<ExpiringContent>(shopButton);
    }
}


void createMultiplayerPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MULTIPLAYER, logicalWidth, 0, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_4_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_4_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_4_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_4_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto titleHandle = textureCache.handle("str/challenges");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 4);

    auto youVsMeButton = createPopupUIButton(registry, parent, UIAction::YOU_VS_ME, 20);
    auto &youVsMeSprite = registry.get<Sprite>(youVsMeButton);
    setSpriteSize(registry, youVsMeButton, 2 * youVsMeSprite.w / 3, 2 * youVsMeSprite.h / 3);
    setPopupUIButtonSize(registry, youVsMeButton, youVsMeSprite.w, youVsMeSprite.h);
    setPos(registry, youVsMeButton, panel.w / 4 - youVsMeSprite.w / 2, panel.h / 2 - youVsMeSprite.h / 3);
    setBoundingBox(registry, youVsMeButton, youVsMeSprite.w, youVsMeSprite.h);

    auto inviteButton = createPopupUIButton(registry, parent, UIAction::INVITE, 20);
    auto &inviteSprite = registry.get<Sprite>(inviteButton);
    setSpriteSize(registry, inviteButton, 2 * inviteSprite.w / 3, 2 * inviteSprite.h / 3);
    setPopupUIButtonSize(registry, inviteButton, inviteSprite.w, inviteSprite.h);
    setPos(registry, inviteButton, panel.w / 2 - inviteSprite.w / 2, panel.h / 2 - inviteSprite.h / 3);
    setBoundingBox(registry, inviteButton, inviteSprite.w, inviteSprite.h);

    auto invitationButton = createPopupUIButton(registry, parent, UIAction::INVITATION, 20);
    auto &invitationSprite = registry.get<Sprite>(invitationButton);
    setSpriteSize(registry, invitationButton, 2 * invitationSprite.w / 3, 2 * invitationSprite.h / 3);
    setPopupUIButtonSize(registry, invitationButton, invitationSprite.w, invitationSprite.h);
    setPos(registry, invitationButton, 3 * panel.w / 4 - invitationSprite.w / 2, panel.h / 2 - invitationSprite.h / 3);
    setBoundingBox(registry, invitationButton, invitationSprite.w, invitationSprite.h);

    auto youVsMeStrHandle = textureCache.handle("str/multiplayer/quick");
    auto youVsMeStrEntity = createSprite(registry, parent, youVsMeStrHandle, 20);
    setPos(registry, youVsMeStrEntity, panel.w / 4 - youVsMeStrHandle->width() / 2, panel.h - 2 * youVsMeStrHandle->height());

    auto inviteStrHandle = textureCache.handle("str/multiplayer/invite");
    auto inviteStrEntity = createSprite(registry, parent, inviteStrHandle, 20);
    setPos(registry, inviteStrEntity, panel.w / 2 - inviteStrHandle->width() / 2, panel.h - 2 * inviteStrHandle->height());

    auto invitationStrHandle = textureCache.handle("str/multiplayer/show");
    auto invitationStrEntity = createSprite(registry, parent, invitationStrHandle, 20);
    setPos(registry, invitationStrEntity, 3 * panel.w / 4 - invitationStrHandle->width() / 2, panel.h - 2 * invitationStrHandle->height());
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

        const auto buttonOffset = (panel.w - (numberOfItems * width + (numberOfItems - 1) * 10)) / 2;

        setPos(registry, entity, buttonOffset + idx * (width + 10), height / 2);
        setSpriteSize(registry, entity, width, height);
        registry.get<Transform>(entity).parent = parent;
    };

    addButton(ItemType::SPEED_UP, 0);
    addButton(ItemType::SLOW_DOWN, 1);
    addButton(ItemType::FOUNTAIN, 2);
    addButton(ItemType::BOMB, 3);
    addButton(ItemType::MONEY, 4);
    addButton(ItemType::RANDOM, 5);

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

    float offset = 0.f;

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


void createGameBottomPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto sym0Handle = toStrSmallHandle(0);
    auto symEmptyHandle = textureCache.handle("str/small/ ");
    auto opponentHandle = Locator::TextureCache::ref().handle("opponent/frame");

    auto parent = createPanel(registry, PanelType::THE_GAME_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 2);
    const auto &panel = registry.get<Panel>(parent);

    auto frame = registry.create();
    registry.assign<Sprite>(frame, opponentHandle, opponentHandle->width(), opponentHandle->height(), opponentFrameRenderingWidth, opponentFrameRenderingHeight);
    registry.assign<Transform>(frame, parent, panel.w - 9 * opponentFrameRenderingWidth / 8.f, panel.h - 13 * opponentFrameRenderingHeight / 12.f);
    registry.assign<Renderable>(frame, 0.f, 160);

    auto opponentScoreEntity = registry.create();
    auto &opponentScoreObserver = registry.assign<PlayerScoreObserver>(opponentScoreEntity);
    float offset = panel.w - (std::extent<decltype(PlayerScoreObserver::entities)>::value + .3f) * sym0Handle->width();
    opponentScoreObserver.local = false;

    for(auto i = 0u; i < std::extent<decltype(PlayerScoreObserver::entities)>::value; ++i) {
        opponentScoreObserver.entities[i] = createHUD(registry, parent, sym0Handle, 160);
        registry.get<HUD>(opponentScoreObserver.entities[i]).handle = i ? symEmptyHandle : sym0Handle;
        setPos(registry, opponentScoreObserver.entities[i], offset, panel.h - 1.2f * sym0Handle->height());
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


void createMultiplayerResultsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::MULTIPLAYER_RESULTS, logicalWidth, 0, logicalWidth, logicalHeight);
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

    auto restartButton = createPopupUIButton(registry, parent, UIAction::MULTIPLAYER, 150);
    const auto &restartSprite = registry.get<Sprite>(restartButton);
    setPos(registry, restartButton, (panel.w - restartSprite.w) / 2, (3 * panel.h / 2 - restartSprite.h) / 2);
    registry.assign<PulseAnimation>(restartButton, 10.f, .5f, .08f, 3000_ui32);
}


void refreshResultsMultiplayerResultsPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    entity_type parent{};

    for(auto entity: registry.view<Panel>()) {
        if(registry.get<Panel>(entity).type == PanelType::MULTIPLAYER_RESULTS) {
            parent = entity;
            break;
        }
    }

    const auto &panel = registry.get<Panel>(parent);

    if(registry.has<PlayerScore>()) {
        const auto &playerScore = registry.get<PlayerScore>();

        auto scoreEntity = createSprite(registry, parent, textureCache.handle("str/score"), 150);
        const auto &scoreSprite = registry.get<Sprite>(scoreEntity);
        setPos(registry, scoreEntity, (panel.w - scoreSprite.w) / 2, panel.h / 3);
        registry.assign<FadeAnimation>(scoreEntity, 0, 255, 500_ui32);
        registry.assign<ExpiringContent>(scoreEntity);

        auto sym0Handle = toStrNormalHandle(0);

        auto printScore = [&](int score, auto x) {
            auto entity = createSprite(registry, parent, sym0Handle, 150);
            registry.get<Sprite>(entity).handle = toStrNormalHandle(score);
            setPos(registry, entity, x, panel.h / 2 - 2 * sym0Handle->height());
            registry.assign<BoundingBox>(entity, sym0Handle->width(), sym0Handle->height());
            registry.assign<FadeAnimation>(entity, 0, 255, 500_ui32);
            registry.assign<ExpiringContent>(entity);
        };

        auto score = playerScore.score;
        printScore(score % 10, 9 * sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, 7 * sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, 5 * sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, 3 * sym0Handle->width() / 2);
        score /= 10;
        printScore(score % 10, 1 * sym0Handle->width() / 2);

        auto vsEntity = createSprite(registry, parent, textureCache.handle("str/vs"), 150);
        const auto &vsSprite = registry.get<Sprite>(vsEntity);
        setPos(registry, vsEntity, (panel.w - vsSprite.w) / 2, panel.h / 2 - 2 * sym0Handle->height());
        registry.assign<FadeAnimation>(vsEntity, 0, 255, 500_ui32);
        registry.assign<ExpiringContent>(vsEntity);

        Locator::GameServices::ref().multiplayer().getOpponentScore([&](auto score) {
            auto printMessage = [&](auto msgEntity) {
                const auto &msgSprite = registry.get<Sprite>(msgEntity);
                setPos(registry, msgEntity, (panel.w - msgSprite.w) / 2, panel.h / 2 + sym0Handle->height());
                registry.assign<FadeAnimation>(msgEntity, 0, 255, 500_ui32);
                registry.assign<ExpiringContent>(msgEntity);
            };

            if(playerScore.score > score) {
                printMessage(createSprite(registry, parent, textureCache.handle("str/won"), 150));
            } else if(playerScore.score < score) {
                printMessage(createSprite(registry, parent, textureCache.handle("str/lose"), 150));
            } else {
                printMessage(createSprite(registry, parent, textureCache.handle("str/draw"), 150));
            }

            printScore(score % 10, panel.w - 3 * sym0Handle->width() / 2);
            score /= 10;
            printScore(score % 10, panel.w - 5 * sym0Handle->width() / 2);
            score /= 10;
            printScore(score % 10, panel.w - 7 * sym0Handle->width() / 2);
            score /= 10;
            printScore(score % 10, panel.w - 9 * sym0Handle->width() / 2);
            score /= 10;
            printScore(score % 10, panel.w - 11 * sym0Handle->width() / 2);
        });
    }
}


void refreshButtonsMultiplayerResultsPanel(Registry &registry) {
    entity_type parent{};

    for(auto entity: registry.view<Panel>()) {
        if(registry.get<Panel>(entity).type == PanelType::MULTIPLAYER_RESULTS) {
            parent = entity;
            break;
        }
    }

    const auto &panel = registry.get<Panel>(parent);

#ifdef DEBUG
    auto shareButton = createPopupUIButton(registry, parent, UIAction::SHARE, 150);
#else
    auto shareButton = createPopupUIButton(registry, parent, UIAction::THE_GAME, 150);
#endif // DEBUG
    const auto &shareSprite = registry.get<Sprite>(shareButton);
    setSpriteSize(registry, shareButton, 4 * shareSprite.w / 5, 4 * shareSprite.h / 5);
    setPopupUIButtonSize(registry, shareButton, shareSprite.w, shareSprite.h);
    setPos(registry, shareButton, (5 * panel.w / 3 - shareSprite.w) / 2, (3 * panel.h / 2 - shareSprite.h) / 2);
    setBoundingBox(registry, shareButton, shareSprite.w, shareSprite.h);
    registry.assign<ExpiringContent>(shareButton);

#ifdef DEBUG
    auto &recorder = Locator::AvRecorder::ref();

    if(recorder.supportExport()) {
        registry.get<Sprite>(shareButton).frame = 1;
    } else {
        registry.get<UIButton>(shareButton).enabled = false;
        registry.get<Sprite>(shareButton).frame = 0;
    }
#endif // DEBUG
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


void createInviteTrainLeft(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::INVITE_TRAIN_LEFT, -logicalWidth, 0, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto trainingHandle = textureCache.handle("str/gameover/training");
    auto trainingEntity = createSprite(registry, parent, trainingHandle, 150);
    setPos(registry, trainingEntity, trainingHandle->width() / 10, (panel.h - trainingHandle->height()) / 2);
}


void createInviteTrainRight(Registry &registry) {
    auto parent = createPanel(registry, PanelType::INVITE_TRAIN_RIGHT, logicalWidth, 0, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto trainingButton = createUIButton(registry, parent, UIAction::TRAINING, 150);
    const auto &trainingSprite = registry.get<Sprite>(trainingButton);
    setSpriteSize(registry, trainingButton, 3 * trainingSprite.w / 5, 3 * trainingSprite.h / 5);
    setPos(registry, trainingButton, panel.w - 4 * trainingSprite.w / 3, (panel.h - trainingSprite.h) / 2);
    setBoundingBox(registry, trainingButton, trainingSprite.w, trainingSprite.h);
}


void createWallpaperPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("target/wallpaper");

    const auto div = 1.f * logicalWidth / handle->width();
    const int w = handle->width() * div;
    const int h = handle->height() * div;

    auto parent = createPanel(registry, PanelType::WALLPAPER, logicalWidth, 0, w, h);

    const auto frame = registry.create();
    registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), w, h);
    registry.assign<Transform>(frame, parent, (logicalWidth - w) / 2.f, 0.f);
    registry.assign<Renderable>(frame, 0.f, 15, 255);
}


void createInviteShareLeft(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::INVITE_SHARE_LEFT, -logicalWidth, 3 * logicalHeight / 4, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto shareHandle = textureCache.handle("str/multiplayer/share");
    auto shareEntity = createSprite(registry, parent, shareHandle, 150);
    setPos(registry, shareEntity, shareHandle->width() / 20, (panel.h - shareHandle->height()) / 2);
}


void createInviteShareRight(Registry &registry) {
    auto parent = createPanel(registry, PanelType::INVITE_SHARE_RIGHT, logicalWidth, 3 * logicalHeight / 4, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto streamButton = createUIButton(registry, parent, UIAction::STREAM, 150);
    auto &streamSprite = registry.get<Sprite>(streamButton);
    setSpriteSize(registry, streamButton, 3 * streamSprite.w / 5, 3 * streamSprite.h / 5);
    setPos(registry, streamButton, panel.w - 6 * streamSprite.w / 5, (panel.h - streamSprite.h) / 2);
    setBoundingBox(registry, streamButton, streamSprite.w, streamSprite.h);
    streamSprite.frame = 1;
}


void createInviteShareTop(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::INVITE_SHARE_TOP, 0, -logicalHeight, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto shareVideoButton = createUIButton(registry, parent, UIAction::SHARE_VIDEO, 150);
    auto &shareVideoSprite = registry.get<Sprite>(shareVideoButton);
    setSpriteSize(registry, shareVideoButton, 3 * shareVideoSprite.w / 5, 3 * shareVideoSprite.h / 5);
    setPos(registry, shareVideoButton, (panel.w - shareVideoSprite.w) / 2, panel.h / 5 - shareVideoSprite.h);
    setBoundingBox(registry, shareVideoButton, shareVideoSprite.w, shareVideoSprite.h);
    registry.assign<PulseAnimation>(shareVideoButton, 10.f, .4f, .1f, 3000_ui32);
    shareVideoSprite.frame = 1;

    auto shareVideoHandle = textureCache.handle("str/share/video");
    auto shareVideoEntity = createSprite(registry, parent, shareVideoHandle, 150);
    setPos(registry, shareVideoEntity, (panel.w - shareVideoHandle->width()) / 2, panel.h / 5 - shareVideoSprite.h - 3 * shareVideoHandle->height() / 2);
}


void createInviteShareBottom(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::INVITE_SHARE_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight);
    const auto &panel = registry.get<Panel>(parent);

    auto shareWallpaperButton = createUIButton(registry, parent, UIAction::SHARE_WALLPAPER, 150);
    auto &shareWallpaperSprite = registry.get<Sprite>(shareWallpaperButton);
    setSpriteSize(registry, shareWallpaperButton, 3 * shareWallpaperSprite.w / 5, 3 * shareWallpaperSprite.h / 5);
    setPos(registry, shareWallpaperButton, (panel.w - shareWallpaperSprite.w) / 2, 4 * panel.h / 5);
    setBoundingBox(registry, shareWallpaperButton, shareWallpaperSprite.w, shareWallpaperSprite.h);
    registry.assign<PulseAnimation>(shareWallpaperButton, 10.f, .4f, .1f, 3000_ui32);
    shareWallpaperSprite.frame = 1;

    auto shareWallpaperHandle = textureCache.handle("str/share/wallpaper");
    auto shareWallpaperEntity = createSprite(registry, parent, shareWallpaperHandle, 150);
    setPos(registry, shareWallpaperEntity, (panel.w - shareWallpaperHandle->width()) / 2, 4 * panel.h / 5 + shareWallpaperSprite.h + shareWallpaperHandle->height() / 2);
}


void createStanza(Registry &registry) {
    registry.assign<Stanza>(registry.create(), logicalWidth / 3, logicalHeight / 3, logicalWidth / 3, logicalHeight / 3, StanzaType::IN_GAME_ITEM_TUTORIAL);
}


void createCamera(Registry &registry) {
    auto camera = registry.create();
    registry.assign<Transform>(camera, camera, 0.f, 0.f);
    registry.assign<Camera>(entt::tag_t{}, camera);
}


void createCameraFrame(Registry &registry) {
    auto handle = Locator::TextureCache::ref().handle("camera/frame");
    int width =  (1.f * handle->width() * logicalHeight) /  (1.f * handle->height());
    int height = logicalHeight;

    if(width < logicalWidth) {
        height =  (1.f * height * logicalWidth) / (1.f * width);
        width = logicalWidth;
    }

    const auto frame = registry.create();
    registry.assign<CameraFrame>(entt::tag_t{}, frame);
    registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), width, height);
    registry.assign<Transform>(frame, frame, (logicalWidth - width) / 2.f, (logicalHeight - height) / 2.f);
    registry.assign<Renderable>(frame, 0.f, 90, 0);
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
