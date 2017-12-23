#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "process_init.h"


namespace gamee {


void ProcessInit::update(delta_type, void *data) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    auto &renderer = *static_cast<GameRenderer *>(data);

    ttfFontCache.load<TTFFontLoader>("ttf/roboto/condensed/48", "font/roboto-condensed.ttf", 48);
    ttfFontCache.load<TTFFontLoader>("ttf/roboto/regular/108", "font/roboto.ttf", 108);
    textureCache.load<SDLTextureLoader>("splash/rainbow", "png/rainbow.png", renderer);

    succeed();
}


}
