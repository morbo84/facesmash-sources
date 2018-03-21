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
        const auto &closeSprite = registry.get<Sprite>(closeButton);
        setSpriteSize(registry, closeButton, 2 * closeSprite.w / 3, 2 * closeSprite.h / 3);
        setPopupUIButtonSize(registry, closeButton, closeSprite.w, closeSprite.h);
        setPos(registry, closeButton, x(closeSprite), panel.h - 3 * closeSprite.h / 8);
        setBoundingBox(registry, closeButton, closeSprite.w, closeSprite.h);
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
        const auto &closeSprite = registry.get<Sprite>(closeButton);
        setSpriteSize(registry, closeButton, 2 * closeSprite.w / 3, 2 * closeSprite.h / 3);
        setPopupUIButtonSize(registry, closeButton, closeSprite.w, closeSprite.h);
        setPos(registry, closeButton, x(closeSprite), 3 * closeSprite.h / 8);
        setBoundingBox(registry, closeButton, closeSprite.w, closeSprite.h);
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
    auto &loginSprite = registry.get<Sprite>(loginButton);
    setPopupUIButtonSize(registry, loginButton, 2 * loginSprite.w / 3, 2 * loginSprite.h / 3);
    setSpriteSize(registry, loginButton, 2 * loginSprite.w / 3, 2 * loginSprite.h / 3);
    setPos(registry, loginButton, loginSprite.w / 8, loginSprite.h / 8);
    loginSprite.frame = 3;
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
    offset += 2 * facesmashHandle->height();

    auto wearegameeHandle = textureCache.handle("str/credits/wearegamee");
    auto wearegameeEntity = createSprite(registry, parent, wearegameeHandle, 20);
    setPos(registry, wearegameeEntity, (panel.w - wearegameeHandle->width()) / 2, offset);
    offset += 9 * wearegameeHandle->height() / 8;

    auto authorsHandle = textureCache.handle("str/credits/authors");
    auto authorsEntity = createSprite(registry, parent, authorsHandle, 20);
    setPos(registry, authorsEntity, (panel.w - authorsHandle->width()) / 2, offset);
    offset += 3 * authorsHandle->height();

    auto emojiHandle = textureCache.handle("str/credits/emoji");
    auto emojiEntity = createSprite(registry, parent, emojiHandle, 20);
    setPos(registry, emojiEntity, (panel.w - emojiHandle->width()) / 2, offset);
    offset += 2 * emojiHandle->height();

    auto emojitwoHandle = textureCache.handle("str/credits/emojitwo");
    auto emojitwoEntity = createSprite(registry, parent, emojitwoHandle, 20);
    setPos(registry, emojitwoEntity, (panel.w - emojitwoHandle->width()) / 2, offset);
    offset += 2 * emojitwoHandle->height();

    auto oslicensesHandle = textureCache.handle("str/credits/oslicenses");
    auto oslicensesEntity = createSprite(registry, parent, oslicensesHandle, 20);
    setPos(registry, oslicensesEntity, (panel.w - oslicensesHandle->width()) / 2, offset);
    offset += 5 * oslicensesHandle->height() / 4;

    auto gpgLicenseButton = createPopupUIButton(registry, parent, UIAction::GPG_LICENSE, 20);
    const auto &gpgLicenseSprite = registry.get<Sprite>(gpgLicenseButton);
    setSpriteSize(registry, gpgLicenseButton, 2 * gpgLicenseSprite.w / 3, 2 * gpgLicenseSprite.h / 3);
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

    auto makingHandle = textureCache.handle("str/support/making");
    auto makingEntity = createSprite(registry, parent, makingHandle, 20);
    setPos(registry, makingEntity, (panel.w - makingHandle->width()) / 2, 1 * panel.h / 6);

    auto timeHandle = textureCache.handle("str/support/time");
    auto timeEntity = createSprite(registry, parent, timeHandle, 20);
    setPos(registry, timeEntity, (panel.w - timeHandle->width()) / 2, 2 * panel.h / 9);

    auto lotHandle = textureCache.handle("str/support/lot");
    auto lotEntity = createSprite(registry, parent, lotHandle, 20);
    setPos(registry, lotEntity, (panel.w - lotHandle->width()) / 2, 3 * panel.h / 9);

    auto coffeeHandle = textureCache.handle("str/support/coffee");
    auto coffeeEntity = createSprite(registry, parent, coffeeHandle, 20);
    setPos(registry, coffeeEntity, (panel.w - coffeeHandle->width()) / 2, 4 * panel.h / 10);

    auto buyButton = createPopupUIButton(registry, parent, UIAction::BUY, 20);
    const auto &buySprite = registry.get<Sprite>(buyButton);
    setPos(registry, buyButton, (panel.w - buySprite.w) / 2, 5 * panel.h / 9);
    registry.get<Sprite>(buyButton).frame = 2;

    auto removeHandle = textureCache.handle("str/support/remove");
    auto removeEntity = createSprite(registry, parent, removeHandle, 20);
    setPos(registry, removeEntity, (panel.w - removeHandle->width()) / 2, 5 * panel.h / 9 + buySprite.h + removeHandle->height() / 3);

    auto offerHandle = textureCache.handle("str/support/offer");
    auto offerEntity = createSprite(registry, parent, offerHandle, 20);
    setPos(registry, offerEntity, (panel.w - offerHandle->width()) / 2, 7 * panel.h / 9);

    auto nextHandle = textureCache.handle("str/support/next");
    auto nextEntity = createSprite(registry, parent, nextHandle, 20);
    setPos(registry, nextEntity, (panel.w - nextHandle->width()) / 2, 8 * panel.h / 9);
}


void refreshSupportPanel(Registry &registry) {
    // TODO
}


void createCameraPermissionPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::CAMERA_PERMISSION, logicalWidth, 0, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_6_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_6_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_6_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_6_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto titleHandle = textureCache.handle("str/camera");
    auto titleEntity = createSprite(registry, parent, titleHandle, 20);
    setPos(registry, titleEntity, (panel.w - titleHandle->width()) / 2, titleHandle->height() / 4);

    auto requiredHandle = textureCache.handle("str/camera/required");
    auto requiredEntity = createSprite(registry, parent, requiredHandle, 20);
    setPos(registry, requiredEntity, (panel.w - requiredHandle->width()) / 2, 2 * panel.h / 6);
}


void refreshCameraPermissionPanel(Registry &registry) {
    auto &permissions = Locator::Permissions::ref();
    entity_type parent{};

    for(auto entity: registry.view<Panel>()) {
        if(registry.get<Panel>(entity).type == PanelType::CAMERA_PERMISSION) {
            parent = entity;
            break;
        }
    }

    const auto &panel = registry.get<Panel>(parent);

    auto cameraButton = createPopupUIButton(registry, parent, UIAction::CAMERA_PERMISSION, 20);
    auto &cameraSprite = registry.get<Sprite>(cameraButton);
    setPos(registry, cameraButton, (panel.w - cameraSprite.w) / 2, panel.h / 2);
    registry.assign<ExpiringContent>(cameraButton);

    auto cameraStatus = permissions.status(PermissionType::CAMERA);

    if(cameraStatus == PermissionStatus::SHOW_RATIONALE) {
        cameraSprite.frame = 0;
    } else {
        registry.get<UIButton>(cameraButton).enabled = false;
        cameraSprite.frame = 3;
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
    setPos(registry, audioButton, panel.w / 4 - audioSprite.w / 2, panel.h / 2 - audioSprite.h / 3);
    audioSprite.frame = audio ? 2 : 3;

    auto videoButton = createPopupUIButton(registry, parent, UIAction::SWITCH_VIDEO, 20);
    auto &videoSprite = registry.get<Sprite>(videoButton);
    setPos(registry, videoButton, panel.w / 2 - videoSprite.w / 2, panel.h / 2 - videoSprite.h / 3);
    videoSprite.frame = video ? 2 : 3;

    auto hapticButton = createPopupUIButton(registry, parent, UIAction::SWITCH_HAPTIC, 20);
    auto &hapticSprite = registry.get<Sprite>(hapticButton);
    setPos(registry, hapticButton, 3 * panel.w / 4 - hapticSprite.w / 2, panel.h / 2 - hapticSprite.h / 3);
    hapticSprite.frame = haptic ? 2 : 3;

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


void createGameBottomPanel(Registry &registry) {
    createPanel(registry, PanelType::THE_GAME_BOTTOM, 0, logicalHeight, logicalWidth, logicalHeight / 4);

    // TODO
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

        auto &recorder = Locator::AvRecorder::ref();
        auto &permissions = Locator::Permissions::ref();

        auto saveButton = createUIButton(registry, parent, UIAction::SAVE, 150);
        const auto &saveSprite = registry.get<Sprite>(saveButton);
        setPos(registry, saveButton, (3 * panel.w / 2 - saveSprite.w) / 2, (3 * panel.h / 2 - saveSprite.h) / 2);
        registry.assign<ExpiringContent>(saveButton);

        if(recorder.supportExport()) {
            const auto status = permissions.status(PermissionType::STORAGE);

            if(status == PermissionStatus::SHOW_RATIONALE) {
                registry.get<UIButton>(saveButton).action = UIAction::STORAGE_PERMISSION;
                registry.get<Sprite>(saveButton).frame = 3;
            } else if(status == PermissionStatus::DENIED) {
                registry.get<UIButton>(saveButton).enabled = false;
                registry.get<Sprite>(saveButton).frame = 3;
            }
        } else {
            registry.get<UIButton>(saveButton).enabled = false;
            registry.get<Sprite>(saveButton).frame = 3;
        }
    }
}


void createTrainingTopPanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto smashHandle = textureCache.handle("str/training/smash");

    auto parent = createPanel(registry, PanelType::TRAINING_TOP, 0, -logicalHeight / 8, logicalWidth, logicalHeight / 8);
    const auto &panel = registry.get<Panel>(parent);

    auto smashEntity = createHUD(registry, parent, smashHandle, 160);
    setPos(registry, smashEntity, .1f * smashHandle->width(), .4f * smashHandle->height());

    auto progressEntity = createProgressBar(registry, parent, 160);
    setSpriteSize(registry, progressEntity, panel.w - 5 * smashHandle->width() / 4, smashHandle->height());
    const auto &progressSprite = registry.get<Sprite>(progressEntity);
    setPos(registry, progressEntity, panel.w - progressSprite.w - .1f * smashHandle->width(), .4f * smashHandle->height());
    registry.assign<FaceProgress>(progressEntity);
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
    };

    createButton(FaceType::ANGRY, 0);
    createButton(FaceType::DISGUSTED, 1);
    createButton(FaceType::FEARFUL, 2);
    createButton(FaceType::HAPPY, 3);
    createButton(FaceType::SAD, 4);
    createButton(FaceType::SURPRISED, 5);
}


void createLoginPleasePanel(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto parent = createPanel(registry, PanelType::LOGIN_PLEASE, 0, logicalHeight, logicalWidth, logicalHeight / 4);
    const auto &panel = registry.get<Panel>(parent);

    auto borderTop = createBoxBorder(registry, parent, BoxBorderType::BOX_2_TOP, 29 * panel.w / 30, 21);
    auto borderBottom = createBoxBorder(registry, parent, BoxBorderType::BOX_2_BOTTOM, 29 * panel.w / 30, 22);
    auto borderLeft = createBoxBorder(registry, parent, BoxBorderType::BOX_2_LEFT, 29 * panel.h / 30, 23);
    auto borderRight = createBoxBorder(registry, parent, BoxBorderType::BOX_2_RIGHT, 29 * panel.h / 30, 24);

    setPos(registry, borderTop, panel.w / 60, panel.h / 60);
    setPos(registry, borderBottom, panel.w / 60, 59 * panel.h / 60 - 5);
    setPos(registry, borderLeft, panel.w / 60, panel.h / 60);
    setPos(registry, borderRight, 59 * panel.w / 60 - 5, panel.h / 60);

    auto oopsHandle = textureCache.handle("str/oops");
    auto oops = createSprite(registry, parent, oopsHandle, 20);
    setPos(registry, oops, (panel.w - oopsHandle->width()) / 2, oopsHandle->height() / 4);

    auto wrongHandle = textureCache.handle("str/wrong");
    auto wrong = createSprite(registry, parent, wrongHandle, 20);
    setPos(registry, wrong, (panel.w - wrongHandle->width()) / 2, panel.h / 2 - wrongHandle->height());

    auto retryHandle = textureCache.handle("str/retry");
    auto retry = createSprite(registry, parent, retryHandle, 20);
    setPos(registry, retry, (panel.w - retryHandle->width()) / 2, panel.h / 2 + retryHandle->height() / 4);

    auto pleaseHandle = textureCache.handle("str/please");
    auto please = createSprite(registry, parent, pleaseHandle, 20);
    setPos(registry, please, (panel.w - pleaseHandle->width()) / 2, panel.h - 3 * pleaseHandle->height() / 2);
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

        angle = -90.f;
    }

    registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), width, height);
    registry.assign<Transform>(frame, frame, (logicalWidth - width) / 2.f, (logicalHeight - height) / 2.f);
    registry.assign<Renderable>(frame, angle, 90, 0);
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
