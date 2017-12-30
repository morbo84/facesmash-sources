#include "../common/constants.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "process_init.h"


namespace gamee {


void ProcessInit::update(delta_type, void *data) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();
    auto &cameraService = Locator::Camera::ref();

    auto &renderer = *static_cast<GameRenderer *>(data);

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
    textureCache.load<SDLTextureLoader>("emoji/rested", "png/rested.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/sad", "png/sad.png", renderer, 128, 128);
    textureCache.load<SDLTextureLoader>("emoji/surprised", "png/surprised.png", renderer, 128, 128);

    const SDL_Color missColor{255_ui8, 0_ui8, 0_ui8, 255_ui8};
    const SDL_Color smashColor{0_ui8, 204_ui8, 0_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("miss/50", "50", renderer, *ttfFontCache.handle("ttf/constant/72"), missColor);
    textureCache.load<TTFFontTextureLoader>("smash/100", "100", renderer, *ttfFontCache.handle("ttf/constant/72"), smashColor);

    const SDL_Color comboColor{120_ui8, 230_ui8, 120_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("combo/x2", "2x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("combo/x3", "3x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("combo/x4", "4x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("combo/x5", "5x COMBO", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);

    textureCache.load<TTFFontTextureLoader>("combo/200", "200", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("combo/300", "300", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("combo/400", "400", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);
    textureCache.load<TTFFontTextureLoader>("combo/500", "500", renderer, *ttfFontCache.handle("ttf/constant/90"), comboColor);

    const SDL_Color hudColor{255_ui8, 255_ui8, 255_ui8, 255_ui8};

    textureCache.load<TTFFontTextureLoader>("hud/score", "SCORE:", renderer, *ttfFontCache.handle("ttf/constant/54"), hudColor);

    if(cameraService.available()) {
        textureCache.load<SDLStreamingTextureLoader>("visage/frame", renderer, cameraService.width(), cameraService.height());
    }

    succeed();
}


}
