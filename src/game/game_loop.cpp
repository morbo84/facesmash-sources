#include <sstream>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "game_renderer.h"
#include "game_loop.h"


namespace gamee {


void GameLoop::loadResources(GameRenderer &renderer) {
    // load all the resources at once

    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    ttfFontCache.load<TTFFontLoader>("ttf/roboto/condensed/48", "font/roboto-condensed.ttf", 48);
    ttfFontCache.load<TTFFontLoader>("ttf/roboto/regular/108", "font/roboto.ttf", 108);

    ttfFontCache.load<TTFFontLoader>("ttf/constant/36", "font/one_constant.ttf", 36);
    ttfFontCache.load<TTFFontLoader>("ttf/constant/54", "font/one_constant.ttf", 54);
    ttfFontCache.load<TTFFontLoader>("ttf/constant/72", "font/one_constant.ttf", 72);
    ttfFontCache.load<TTFFontLoader>("ttf/constant/90", "font/one_constant.ttf", 90);

    const SDL_Color bannerColor{205_ui8, 205_ui8, 205_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("logo/division", "Cynny Game Division", renderer, *ttfFontCache.handle("ttf/roboto/condensed/48"), bannerColor);
    textureCache.load<TTFFontTextureLoader>("logo/gamee", "GAMEE", renderer, *ttfFontCache.handle("ttf/roboto/regular/108"), bannerColor);
    textureCache.load<SDLTextureLoader>("logo/rainbow", "png/logo/rainbow.png", renderer, 748, 33);

    textureCache.load<SDLTextureLoader>("game/explosion", "png/gui/explosion.png", renderer, 960, 768);

    textureCache.load<SDLTextureLoader>("emoji/angry", "png/emoji/angry.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("emoji/disgusted", "png/emoji/disgusted.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("emoji/fearful", "png/emoji/fearful.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("emoji/happy", "png/emoji/happy.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("emoji/sad", "png/emoji/sad.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("emoji/surprised", "png/emoji/surprised.png", renderer, 180, 180);

    textureCache.load<SDLTextureLoader>("emoji/malus", "png/emoji/malus.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("emoji/bonus", "png/emoji/bonus.png", renderer, 180, 180);

    const SDL_Color missColor{255_ui8, 0_ui8, 0_ui8, 255_ui8};
    const SDL_Color smashColor{0_ui8, 204_ui8, 0_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("miss/10", "10", renderer, *ttfFontCache.handle("ttf/constant/72"), missColor);
    textureCache.load<TTFFontTextureLoader>("miss/20", "20", renderer, *ttfFontCache.handle("ttf/constant/72"), missColor);
    textureCache.load<TTFFontTextureLoader>("miss/50", "50", renderer, *ttfFontCache.handle("ttf/constant/72"), missColor);
    textureCache.load<TTFFontTextureLoader>("miss/100", "100", renderer, *ttfFontCache.handle("ttf/constant/72"), missColor);
    textureCache.load<TTFFontTextureLoader>("smash/50", "50", renderer, *ttfFontCache.handle("ttf/constant/72"), smashColor);
    textureCache.load<TTFFontTextureLoader>("smash/100", "100", renderer, *ttfFontCache.handle("ttf/constant/72"), smashColor);
    textureCache.load<TTFFontTextureLoader>("smash/200", "200", renderer, *ttfFontCache.handle("ttf/constant/72"), smashColor);
    textureCache.load<TTFFontTextureLoader>("smash/250", "250", renderer, *ttfFontCache.handle("ttf/constant/72"), smashColor);

    const SDL_Color comboColor{120_ui8, 230_ui8, 120_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("bonus/perfect", "PERFECT", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x2", "2x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x3", "3x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x4", "4x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x5", "5x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);

    const SDL_Color hudColor{255_ui8, 255_ui8, 255_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("hud/score", "SCORE:", renderer, *ttfFontCache.handle("ttf/constant/54"), hudColor);
    textureCache.load<TTFFontTextureLoader>("hud/time", "TIME:", renderer, *ttfFontCache.handle("ttf/constant/54"), hudColor);

#ifdef CAMERA_FRAME_AVAILABLE
    auto &cameraService = Locator::Camera::ref();
    textureCache.load<SDLStreamingTextureLoader>("camera/frame", renderer, cameraService.width(), cameraService.height());
#endif // CAMERA_FRAME_AVAILABLE

    textureCache.load<SDLTextureLoader>("button/sound", "png/gui/sound.png", renderer, 160, 160);
    textureCache.load<SDLTextureLoader>("button/mute", "png/gui/mute.png", renderer, 160, 160);

    textureCache.load<SDLTextureLoader>("gui/popup", "png/gui/popup.png", renderer, 720, 870);
    textureCache.load<SDLTextureLoader>("gui/ribbon", "png/gui/ribbon.png", renderer, 900, 360);
    textureCache.load<SDLTextureLoader>("gui/patch", "png/gui/patch.png", renderer, 1080, 960);

    textureCache.load<SDLTextureLoader>("button/reload", "png/gui/reload.png", renderer, 160, 160);
    textureCache.load<SDLTextureLoader>("button/share", "png/gui/share.png", renderer, 160, 160);
    textureCache.load<SDLTextureLoader>("button/menu", "png/gui/menu.png", renderer, 160, 160);
}


void GameLoop::createSplashScreen() {
    // we create it and make the entities already "ready to run"
    // after all we are going to show the splash screen immediately, right?

    auto &textureCache = Locator::TextureCache::ref();
    auto rainbow = textureCache.handle("logo/rainbow");

    auto panel = registry.create();
    registry.assign<Panel>(panel, logicalWidth, logicalHeight, PanelType::SPLASH_SCREEN);
    registry.assign<Transform>(panel, panel, 0.f, 0.f);

    auto orange = registry.create<Renderable>();
    registry.assign<Sprite>(orange, rainbow, 150, 33, 80, 20, 0, 0);
    registry.assign<Transform>(orange, panel, logicalWidth / 2.f - 200, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(orange, 0, 255, 400_ui32);

    auto yellow = registry.create<Renderable>();
    registry.assign<Sprite>(yellow, rainbow, 150, 33, 80, 20, 150, 0);
    registry.assign<Transform>(yellow, panel, logicalWidth / 2.f - 120, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(yellow, 0, 255, 800_ui32);

    auto green = registry.create<Renderable>();
    registry.assign<Sprite>(green, rainbow, 150, 33, 80, 20, 300, 0);
    registry.assign<Transform>(green, panel, logicalWidth / 2.f - 40, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(green, 0, 255, 1200_ui32);

    auto blue = registry.create<Renderable>();
    registry.assign<Sprite>(blue, rainbow, 150, 33, 80, 20, 450, 0);
    registry.assign<Transform>(blue, panel, logicalWidth / 2.f + 40, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(blue, 0, 255, 1600_ui32);

    auto purple = registry.create<Renderable>();
    registry.assign<Sprite>(purple, rainbow, 150, 33, 80, 20, 600, 0);
    registry.assign<Transform>(purple, panel, logicalWidth / 2.f + 120, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(purple, 0, 255, 2000_ui32);

    auto gamee = Locator::TextureCache::ref().handle("logo/gamee");
    auto company = registry.create<Renderable>();
    registry.assign<Sprite>(company, gamee, gamee->width(), gamee->height(), gamee->width(), gamee->height());
    registry.assign<Transform>(company, panel, logicalWidth / 2.f - gamee->width() / 2.f, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(company, 0, 255, 2000_ui32);

    auto division = Locator::TextureCache::ref().handle("logo/division");
    auto dep = registry.create<Renderable>();
    registry.assign<Sprite>(dep, division, division->width(), division->height(), division->width(), division->height());
    registry.assign<Transform>(dep, panel, logicalWidth / 2.f - division->width() / 2.f, logicalHeight / 2.f + 60);
    registry.assign<FadeAnimation>(dep, 0, 255, 2000_ui32);
}


void GameLoop::createTopPatch() {
    auto &textureCache = Locator::TextureCache::ref();
    auto patchHandle = textureCache.handle("gui/patch");

    auto patch = registry.create<Renderable>();
    registry.assign<Sprite>(patch, patchHandle, patchHandle->width(), patchHandle->height(), patchHandle->width(), patchHandle->height());
    registry.assign<Transform>(patch, patch, 0.f, -logicalHeight / 2.f);
    registry.assign<Panel>(patch, logicalWidth, logicalHeight / 2, PanelType::TOP_PATCH);
}


void GameLoop::createBottomPatch() {
    auto &textureCache = Locator::TextureCache::ref();
    auto patchHandle = textureCache.handle("gui/patch");

    auto patch = registry.create<Renderable>();
    registry.assign<Sprite>(patch, patchHandle, patchHandle->width(), patchHandle->height(), patchHandle->width(), patchHandle->height());
    registry.assign<Transform>(patch, patch, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(patch, logicalWidth, logicalHeight / 2, PanelType::BOTTOM_PATCH);
}


void GameLoop::createMenuTopPanel() {
    auto &textureCache = Locator::TextureCache::ref();
    auto &audioService = Locator::Audio::ref();

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, -logicalHeight / 2.f);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::MENU_TOP_PANEL);

    auto audioButton = registry.create();
    auto audioButtonHandle = audioService.isMute() ? textureCache.handle("button/mute") : textureCache.handle("button/sound");
    registry.assign<Sprite>(audioButton, audioButtonHandle, audioButtonHandle->width(), audioButtonHandle->height(), audioButtonHandle->width(), audioButtonHandle->height());
    registry.assign<Renderable>(audioButton, 0.f, 150);
    registry.assign<Transform>(audioButton, panel, logicalWidth - 3.f * audioButtonHandle->width() / 2.f, audioButtonHandle->height() / 2.f);
    registry.assign<UIButton>(audioButton, UIAction::SWITCH_AUDIO);
}


void GameLoop::createMenuBottomPanel() {
    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::MENU_BOTTOM_PANEL);
}


void GameLoop::createPlayButton() {
    auto &textureCache = Locator::TextureCache::ref();

    auto entity = registry.create();
    auto emoji = textureCache.handle("emoji/happy");

    registry.assign<Renderable>(entity, 0.f, 160, 0);
    registry.assign<Transform>(entity, entity, logicalWidth / 2.f - emoji->width() / 2.f, logicalHeight / 2.f - emoji->height() / 2.f);
    registry.assign<Sprite>(entity, emoji, emoji->width(), emoji->height(), emoji->width(), emoji->height());
    registry.assign<UIButton>(entity, UIAction::PLAY);
    registry.attach<PlayButton>(entity);
}


void GameLoop::createCameraFrame() {
#ifdef CAMERA_FRAME_AVAILABLE
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
#endif // CAMERA_FRAME_AVAILABLE
}


void GameLoop::createGameTopPanel(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();
    auto font = Locator::TTFFontCache::ref().handle("ttf/constant/54");

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, -logicalHeight / 2.f);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::GAME_TOP_PANEL);

    auto scoreLabel = registry.create();
    auto scoreLabelHandle = textureCache.handle("hud/score");
    registry.assign<Renderable>(scoreLabel, 0.f, 160);
    registry.assign<HUD>(scoreLabel, scoreLabelHandle, scoreLabelHandle->width(), scoreLabelHandle->height(), scoreLabelHandle->width(), scoreLabelHandle->height());
    registry.assign<Transform>(scoreLabel, panel, 32.f, 32.f);

    auto score = registry.create();

    std::stringstream scoreStream;
    scoreStream << (registry.attach<PlayerScore>(score).score);
    auto scoreHandle = textureCache.temp<TTFFontTextureLoader>(scoreStream.str().c_str(), renderer, font.get(), SDL_Color{ 255, 255, 255, 255 });
    registry.assign<Renderable>(score, 0.f, 160);
    registry.assign<Transform>(score, panel, 32.f + scoreLabelHandle->width() + 16.f, 32.f);
    registry.assign<HUD>(score, scoreHandle, scoreHandle->width(), scoreHandle->height(), scoreHandle->width(), scoreHandle->height());

    auto timeLabel = registry.create();
    auto timeLabelHandle = textureCache.handle("hud/time");
    registry.assign<Renderable>(timeLabel, 0.f, 160);
    registry.assign<HUD>(timeLabel, timeLabelHandle, timeLabelHandle->width(), timeLabelHandle->height(), timeLabelHandle->width(), timeLabelHandle->height());
    registry.assign<Transform>(timeLabel, panel, logicalWidth - 2.f * timeLabelHandle->width(), 32.f);

    auto time = registry.create();
    std::stringstream timeStream;
    timeStream << (registry.attach<GameTimer>(time).remaining / 1000);
    auto timeHandle = textureCache.temp<TTFFontTextureLoader>(timeStream.str().c_str(), renderer, font.get(), SDL_Color{ 255, 255, 255, 255 });
    registry.assign<Renderable>(time, 0.f, 160);
    registry.assign<Transform>(time, panel, logicalWidth - timeLabelHandle->width() + 16.f, 32.f);
    registry.assign<HUD>(time, timeHandle, timeHandle->width(), timeHandle->height(), timeHandle->width(), timeHandle->height());

    auto bonus = registry.create();
    registry.assign<Renderable>(bonus, 0.f, 160);
    registry.assign<Transform>(bonus, bonus, logicalWidth / 2.f, 152.f);
    registry.attach<BonusSmash>(bonus);
}


void GameLoop::createGameBottomPanel() {
    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::GAME_BOTTOM_PANEL);
}


void GameLoop::createGameOverPanel() {
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


void GameLoop::createCamera() {
    auto camera = registry.create();
    registry.assign<Transform>(camera, camera, 0.f, 0.f);
    registry.attach<Camera>(camera);
}


#ifdef DEBUG
void GameLoop::createSmashButtons() {
    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, logicalWidth + 96.f, logicalHeight / 2.f - 368.f);
    registry.assign<Panel>(panel, 96, 736, PanelType::SMASH_BUTTONS_PANEL);

    auto addButton = [this, panel](FaceType type, TextureCache::resource_type emoji, int idx) {
        auto &textureCache = Locator::TextureCache::ref();
        auto button = registry.create();
        auto handle = textureCache.handle(emoji);

        registry.assign<SmashButton>(button, type);
        registry.assign<Transform>(button, panel, 0.f, idx * (96.f + 32.f));
        registry.assign<Sprite>(button, handle, handle->width(), handle->height(), 96, 96);
        registry.assign<Renderable>(button, 0.f, 220);
        registry.assign<BoundingBox>(button, 96, 96);
    };

    addButton(FaceType::ANGRY, "emoji/angry", 0);
    addButton(FaceType::DISGUSTED, "emoji/disgusted", 1);
    addButton(FaceType::HAPPY, "emoji/happy", 2);
    addButton(FaceType::SURPRISED, "emoji/surprised", 3);
    addButton(FaceType::FEARFUL, "emoji/fearful", 4);
    addButton(FaceType::SAD, "emoji/sad", 5);
}


void GameLoop::createDebugStuff() {
    registry.attach<TimeDebug>(registry.create(Renderable{0.f, 255}), 0.f);
    registry.attach<FPSDebug>(registry.create(Renderable{0.f, 255}));
}
#endif // DEBUG


void GameLoop::init(GameRenderer &renderer) {
    loadResources(renderer);

    createSplashScreen();
    createTopPatch();
    createBottomPatch();
    createMenuTopPanel();
    createMenuBottomPanel();
    createPlayButton();
    createCameraFrame();
    createGameTopPanel(renderer);
    createGameBottomPanel();
    createGameOverPanel();
    createCamera();

#ifdef DEBUG
    createSmashButtons();
    createDebugStuff();
#endif // DEBUG

    // request immediately a transition to the main menu from the splash screen
    Locator::Dispatcher::ref().trigger<SceneChangeEvent>(SceneType::SPLASH_SCREEN);
}



void GameLoop::close() {
    // reset cache (and avoid crashes)
    Locator::TextureCache::ref().clear();
    Locator::TTFFontCache::ref().clear();
}


void GameLoop::update(GameRenderer &renderer, delta_type delta) {
    renderer.clear();

    // sum what remains from the previous step
    accumulator += delta;

    sceneSystem.update(registry, delta);
    uiButtonSystem.update(registry);

#ifdef DEBUG
    smashButtonSystem.update(registry);
#endif // DEBUG

    destroyLaterSystem.update(registry, delta);
    faceSmashSystem.update(registry);
    bonusSystem.update(registry);
    faceModifierSystem.update(registry, delta);

    // invoke systems at 50 fps (but for rendering and few other systems)
    while(accumulator >= msPerUpdate) {
        movementSystem.update(registry, msPerUpdate);
        // consume a token
        accumulator -= msPerUpdate;
    }

    theGameSystem.update(registry);
    animationSystem.update(registry, delta);
    scoreSystem.update(registry, renderer, delta);
    timerSystem.update(registry, renderer, delta);
    cameraSystem.update(registry, delta);

    frameSystem.update(registry);

    renderingSystem.update(registry, renderer);
    hudSystem.update(registry, renderer, delta);

    renderer.present();
}


}
