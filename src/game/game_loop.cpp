#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "game_renderer.h"
#include "game_loop.h"


namespace gamee {


void GameLoop::init(GameRenderer &renderer) {
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
    textureCache.load<SDLTextureLoader>("logo/rainbow", "png/rainbow.png", renderer, 748, 33);

    textureCache.load<SDLTextureLoader>("game/explosion", "png/explosion.png", renderer, 960, 768);

    textureCache.load<SDLTextureLoader>("emoji/angry", "png/angry.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/disgusted", "png/disgusted.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/fearful", "png/fearful.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/happy", "png/happy.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/sad", "png/sad.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/surprised", "png/surprised.png", renderer, 128, 128);

    textureCache.load<SDLTextureLoader>("emoji/pouting", "png/pouting.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/star", "png/star.png", renderer, 128, 128);

    textureCache.load<SDLTextureLoader>("icon/end", "png/end.png", renderer, 128, 128);

    const SDL_Color missColor{255_ui8, 0_ui8, 0_ui8, 255_ui8};
    const SDL_Color smashColor{0_ui8, 204_ui8, 0_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("miss/50", "50", renderer, *ttfFontCache.handle("ttf/constant/72"), missColor);
    textureCache.load<TTFFontTextureLoader>("smash/100", "100", renderer, *ttfFontCache.handle("ttf/constant/72"), smashColor);

    const SDL_Color comboColor{120_ui8, 230_ui8, 120_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("bonus/perfect", "PERFECT", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x2", "2x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x3", "3x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x4", "4x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("bonus/x5", "5x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);

    const SDL_Color hudColor{255_ui8, 255_ui8, 255_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("hud/score", "SCORE:", renderer, *ttfFontCache.handle("ttf/constant/54"), hudColor);
    textureCache.load<TTFFontTextureLoader>("hud/time", "TIME:", renderer, *ttfFontCache.handle("ttf/constant/54"), hudColor);

#ifdef __ANDROID__
    auto &cameraService = Locator::Camera::ref();
    textureCache.load<SDLStreamingTextureLoader>("camera/frame", renderer, cameraService.width(), cameraService.height());
#endif

    textureCache.load<SDLTextureLoader>("game/banner", "png/banner.png", renderer, 1080, 320);

    textureCache.load<SDLTextureLoader>("menu/bg", "png/menu_bg.png", renderer, 1080, 1920);

    textureCache.load<SDLTextureLoader>("end/reload", "png/reload.png", renderer, 128, 128);

    // request the splash screen

    registry.attach<SceneChangeRequest>(registry.create(), SceneType::SPLASH_SCREEN);
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

    frameSystem.update();

    renderingSystem.update(registry, renderer);
    hudSystem.update(registry, renderer, delta);

    renderer.present();
}


}
