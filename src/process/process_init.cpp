#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "process_init.h"


namespace gamee {


void ProcessInit::update(delta_type, void *data) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &ttfFontTextureCache = Locator::TTFFontTextureCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    auto &renderer = *static_cast<GameRenderer *>(data);

    ttfFontCache.load<TTFFontLoader>("ttf/roboto/condensed/48", "font/roboto-condensed.ttf", 48);
    ttfFontTextureCache.load<TTFFontTextureLoader>("logo/division", "Cynny Game Division", renderer, *ttfFontCache.handle("ttf/roboto/condensed/48"), SDL_Color{205, 205, 205, 205});

    ttfFontCache.load<TTFFontLoader>("ttf/roboto/regular/108", "font/roboto.ttf", 108);
    ttfFontTextureCache.load<TTFFontTextureLoader>("logo/gamee", "GAMEE", renderer, *ttfFontCache.handle("ttf/roboto/regular/108"), SDL_Color{205, 205, 205, 205});

    textureCache.load<SDLTextureLoader>("logo/rainbow", "png/rainbow.png", renderer);
    textureCache.load<SDLTextureLoader>("game/explosion", "png/explosion.png", renderer);

    succeed();
}


}
