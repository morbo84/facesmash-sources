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

    ttfFontCache.load<TTFFontLoader>("font/default/small", "font/roboto-condensed.ttf", 48);
    ttfFontCache.load<TTFFontLoader>("font/default/large", "font/roboto.ttf", 108);

    ttfFontCache.load<TTFFontLoader>("font/debug/small", "font/one_constant.ttf", 54);
    ttfFontCache.load<TTFFontLoader>("font/debug/normal", "font/one_constant.ttf", 72);
    ttfFontCache.load<TTFFontLoader>("font/debug/large", "font/one_constant.ttf", 90);

    const SDL_Color bannerColor{205_ui8, 205_ui8, 205_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("logo/division", "Cynny Game Division", renderer, *ttfFontCache.handle("font/default/small"), bannerColor);
    textureCache.load<TTFFontTextureLoader>("logo/gamee", "GAMEE", renderer, *ttfFontCache.handle("font/default/large"), bannerColor);
    textureCache.load<SDLTextureLoader>("logo/rainbow", "png/logo/rainbow.png", renderer, 748, 33);

    textureCache.load<SDLTextureLoader>("game/explosion", "png/gui/explosion.png", renderer, 960, 768);

    textureCache.load<SDLTextureLoader>("face/angry", "png/emoji/emojitwo/angry.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("face/disgusted", "png/emoji/emojitwo/disgusted.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("face/fearful", "png/emoji/emojitwo/fearful.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("face/happy", "png/emoji/emojitwo/happy.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("face/sad", "png/emoji/emojitwo/sad.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("face/surprised", "png/emoji/emojitwo/surprised.png", renderer, 180, 180);

    textureCache.load<SDLTextureLoader>("item/speed_up", "png/emoji/emojitwo/fire.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("item/slow_down", "png/emoji/emojitwo/hourglass.png", renderer, 180, 180);
    textureCache.load<SDLTextureLoader>("item/fountain", "png/emoji/emojitwo/pill.png", renderer, 180, 180);

    const SDL_Color missColor{255_ui8, 0_ui8, 0_ui8, 255_ui8};
    const SDL_Color smashColor{0_ui8, 204_ui8, 0_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("miss/10", "10", renderer, *ttfFontCache.handle("font/debug/normal"), missColor);
    textureCache.load<TTFFontTextureLoader>("miss/20", "20", renderer, *ttfFontCache.handle("font/debug/normal"), missColor);
    textureCache.load<TTFFontTextureLoader>("miss/50", "50", renderer, *ttfFontCache.handle("font/debug/normal"), missColor);
    textureCache.load<TTFFontTextureLoader>("miss/100", "100", renderer, *ttfFontCache.handle("font/debug/normal"), missColor);
    textureCache.load<TTFFontTextureLoader>("smash/50", "50", renderer, *ttfFontCache.handle("font/debug/normal"), smashColor);
    textureCache.load<TTFFontTextureLoader>("smash/100", "100", renderer, *ttfFontCache.handle("font/debug/normal"), smashColor);
    textureCache.load<TTFFontTextureLoader>("smash/200", "200", renderer, *ttfFontCache.handle("font/debug/normal"), smashColor);
    textureCache.load<TTFFontTextureLoader>("smash/250", "250", renderer, *ttfFontCache.handle("font/debug/normal"), smashColor);

    const SDL_Color comboColor{120_ui8, 230_ui8, 120_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("reward/perfect", "PERFECT", renderer, *ttfFontCache.handle("font/debug/large"), comboColor);
    textureCache.load<TTFFontTextureLoader>("reward/x2", "2x COMBO", renderer, *ttfFontCache.handle("font/debug/large"), comboColor);
    textureCache.load<TTFFontTextureLoader>("reward/x3", "3x COMBO", renderer, *ttfFontCache.handle("font/debug/large"), comboColor);
    textureCache.load<TTFFontTextureLoader>("reward/x4", "4x COMBO", renderer, *ttfFontCache.handle("font/debug/large"), comboColor);
    textureCache.load<TTFFontTextureLoader>("reward/x5", "5x COMBO", renderer, *ttfFontCache.handle("font/debug/large"), comboColor);

    const SDL_Color hudColor{255_ui8, 255_ui8, 255_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("hud/score", "SCORE:", renderer, *ttfFontCache.handle("font/debug/small"), hudColor);
    textureCache.load<TTFFontTextureLoader>("hud/time", "TIME:", renderer, *ttfFontCache.handle("font/debug/small"), hudColor);

#ifdef CAMERA_FRAME_AVAILABLE
    auto &cameraService = Locator::Camera::ref();
    textureCache.load<SDLStreamingTextureLoader>("camera/frame", renderer, cameraService.width(), cameraService.height());
#endif // CAMERA_FRAME_AVAILABLE

    textureCache.load<SDLTextureLoader>("button/empty", "png/gui/button.png", renderer, 160, 160);
    textureCache.load<SDLTextureLoader>("button/sound", "png/gui/sound.png", renderer, 160, 160);
    textureCache.load<SDLTextureLoader>("button/mute", "png/gui/mute.png", renderer, 160, 160);
    textureCache.load<SDLTextureLoader>("button/credits", "png/gui/info.png", renderer, 160, 160);

    const SDL_Color tutorialColor{255_ui8, 255_ui8, 255_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("tutorial/face", "USE YOUR FACE", renderer, *ttfFontCache.handle("font/debug/large"), tutorialColor);
    textureCache.load<TTFFontTextureLoader>("tutorial/touch", "USE YOUR FINGER", renderer, *ttfFontCache.handle("font/debug/large"), tutorialColor);

    textureCache.load<SDLTextureLoader>("palette/bg_top", "png/palette/ontheroad/bg_top.png", renderer, 1080, 320);
    textureCache.load<SDLTextureLoader>("palette/bg_middle", "png/palette/ontheroad/bg_middle.png", renderer, 1080, 320);
    textureCache.load<SDLTextureLoader>("palette/bg_bottom", "png/palette/ontheroad/bg_bottom.png", renderer, 1080, 320);
    textureCache.load<SDLTextureLoader>("palette/separator", "png/palette/ontheroad/separator.png", renderer, 900, 8);

    textureCache.load<SDLTextureLoader>("gui/window", "png/gui/window.png", renderer, 720, 870);
    textureCache.load<SDLTextureLoader>("gui/popup", "png/gui/popup.png", renderer, 720, 870);
    textureCache.load<SDLTextureLoader>("gui/ribbon", "png/gui/ribbon.png", renderer, 900, 360);

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


void GameLoop::createBackgroundTopPanel() {
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


void GameLoop::createBackgroundBottomPanel() {
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


void GameLoop::createMenuBottomPanel() {
    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::MENU_BOTTOM_PANEL);
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


void GameLoop::createCreditsPanel() {
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


void GameLoop::createTutorialTopPanel() {
    auto &textureCache = Locator::TextureCache::ref();
    auto refHandle = textureCache.handle("button/empty");

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, -logicalHeight / 2.f);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::TUTORIAL_TOP_PANEL);

    auto addButton = [this, panel, &textureCache](TextureCache::resource_type face, int idx) {
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
    auto handle = textureCache.handle("tutorial/face");
    registry.assign<Renderable>(entity, 0.f, 150);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, panel, logicalWidth / 2.f - handle->width() / 2.f, logicalHeight / 2.f - 3.f * refHandle->height() / 2.f - handle->height() - 10.f);
}


void GameLoop::createTutorialBottomPanel() {
    auto &textureCache = Locator::TextureCache::ref();
    auto refHandle = textureCache.handle("button/empty");

    auto panel = registry.create();
    registry.assign<Transform>(panel, panel, 0.f, 1.f * logicalHeight);
    registry.assign<Panel>(panel, logicalWidth, logicalHeight / 2, PanelType::TUTORIAL_BOTTOM_PANEL);

    auto addButton = [this, panel, &textureCache](TextureCache::resource_type face, int idx) {
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
    auto handle = textureCache.handle("tutorial/touch");
    registry.assign<Renderable>(entity, 0.f, 150);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, panel, logicalWidth / 2.f - handle->width() / 2.f, 3.f * refHandle->height() / 2.f + 10.f);
}


void GameLoop::createGameTopPanel(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();
    auto font = Locator::TTFFontCache::ref().handle("font/debug/small");

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

    auto reward = registry.create();
    registry.assign<Renderable>(reward, 0.f, 160);
    registry.assign<Transform>(reward, reward, logicalWidth / 2.f, 152.f);
    registry.attach<Reward>(reward);
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


void GameLoop::createTrainingTopPanel() {
    // TODO
}


void GameLoop::createTrainingBottomPanel() {
    // TODO
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

    auto addButton = [this, panel](FaceType type, TextureCache::resource_type face, int idx) {
        auto &textureCache = Locator::TextureCache::ref();
        auto button = registry.create();
        auto handle = textureCache.handle(face);

        registry.assign<SmashButton>(button, type);
        registry.assign<Transform>(button, panel, 0.f, idx * (96.f + 32.f));
        registry.assign<Sprite>(button, handle, handle->width(), handle->height(), 96, 96);
        registry.assign<Renderable>(button, 0.f, 220);
        registry.assign<BoundingBox>(button, 96, 96);
    };

    addButton(FaceType::ANGRY, "face/angry", 0);
    addButton(FaceType::DISGUSTED, "face/disgusted", 1);
    addButton(FaceType::HAPPY, "face/happy", 2);
    addButton(FaceType::SURPRISED, "face/surprised", 3);
    addButton(FaceType::FEARFUL, "face/fearful", 4);
    addButton(FaceType::SAD, "face/sad", 5);
}


void GameLoop::createDebugStuff() {
    registry.attach<TimeDebug>(registry.create(Renderable{0.f, 255}));
    registry.attach<FPSDebug>(registry.create(Renderable{0.f, 255}));
}
#endif // DEBUG


void GameLoop::init(GameRenderer &renderer) {
    loadResources(renderer);

    createSplashScreen();
    createBackgroundTopPanel();
    createBackgroundBottomPanel();
    createMenuTopPanel();
    createMenuBottomPanel();
    createCameraFrame();
    createCreditsPanel();
    createTutorialTopPanel();
    createTutorialBottomPanel();
    createGameTopPanel(renderer);
    createGameBottomPanel();
    createGameOverPanel();
    createTrainingTopPanel();
    createTrainingBottomPanel();
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
    accumulator50FPS += delta;
    accumulator20FPS += delta;

    sceneSystem.update(registry, delta);
    uiButtonSystem.update(registry);

#ifdef DEBUG
    smashButtonSystem.update(registry);
#endif // DEBUG

    itemSystem.update(registry, factory, delta);
    faceSmashSystem.update(registry, factory);
    rewardSystem.update(registry);

    destroyLaterSystem.update(registry, delta);

    // invoke systems at 50 fps
    while(accumulator50FPS >= msPerUpdate50FPS) {
        movementSystem.update(registry, msPerUpdate50FPS);
        frameSystem.update(registry);
        // consume a token
        accumulator50FPS -= msPerUpdate50FPS;
    }

    // invoke systems at 20 fps
    while(accumulator20FPS >= msPerUpdate20FPS) {
        scoreSystem.update(registry, renderer);
        timerSystem.update(registry, renderer, msPerUpdate20FPS);
        cameraSystem.update(registry, msPerUpdate20FPS);
        // consume a token
        accumulator20FPS -= msPerUpdate20FPS;
    }

    theGameSystem.update(registry, factory);
    trainingSystem.update(registry, factory, delta);
    animationSystem.update(registry, delta);

    renderingSystem.update(registry, renderer);
    hudSystem.update(registry, renderer, delta);

    renderer.present();
}


}
