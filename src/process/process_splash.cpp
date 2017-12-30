#include <SDL_rect.h>
#include <SDL_pixels.h>
#include <SDL_thread.h>
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "process_splash.h"


namespace gamee {


void ProcessSplash::updateRainbow(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();

    auto handle = textureCache.handle("logo/rainbow");

    SDL_Rect orangeSrc{0, 0, 150, 33};
    SDL_Rect orangeDst{logicalWidth / 2 - 200, logicalHeight / 2 - 60, 80, 20};

    SDL_Rect yellowSrc{150, 0, 150, 33};
    SDL_Rect yellowDst{logicalWidth / 2 - 120, logicalHeight / 2 - 60, 80, 20};

    SDL_Rect greenSrc{300, 0, 150, 33};
    SDL_Rect greenDst{logicalWidth / 2 - 40, logicalHeight / 2 - 60, 80, 20};

    SDL_Rect blueSrc{450, 0, 150, 33};
    SDL_Rect blueDst{logicalWidth / 2 + 40, logicalHeight / 2 - 60, 80, 20};

    SDL_Rect purpleSrc{600, 0, 148, 33};
    SDL_Rect purpleDst{logicalWidth / 2 + 120, logicalHeight / 2 - 60, 80, 20};

    SDL_SetTextureBlendMode(*handle, SDL_BLENDMODE_BLEND);

    SDL_SetTextureAlphaMod(*handle, 255 * (elapsed / (logo/5) > 1 ? 1 : (elapsed / (logo/5))));
    SDL_RenderCopy(renderer, *handle, &orangeSrc, &orangeDst);

    SDL_SetTextureAlphaMod(*handle, 255 * (elapsed / (2*logo/5) > 1 ? 1 : (elapsed / (2*logo/5))));
    SDL_RenderCopy(renderer, *handle, &yellowSrc, &yellowDst);

    SDL_SetTextureAlphaMod(*handle, 255 * (elapsed / (3*logo/5) > 1 ? 1 : (elapsed / (3*logo/5))));
    SDL_RenderCopy(renderer, *handle, &greenSrc, &greenDst);

    SDL_SetTextureAlphaMod(*handle, 255 * (elapsed / (4*logo/5) > 1 ? 1 : (elapsed / (4*logo/5))));
    SDL_RenderCopy(renderer, *handle, &blueSrc, &blueDst);

    SDL_SetTextureAlphaMod(*handle, 255 * (elapsed / logo > 1 ? 1 : (elapsed / logo)));
    SDL_RenderCopy(renderer, *handle, &purpleSrc, &purpleDst);
}


void ProcessSplash::updateGamee(GameRenderer &renderer) {
    auto handle = Locator::TextureCache::ref().handle("logo/gamee");

    SDL_Rect dst;
    dst.w = handle->width();
    dst.h = handle->height();
    dst.x = logicalWidth / 2 - dst.w / 2;
    dst.y = logicalHeight / 2 - 60;

    SDL_SetTextureBlendMode(*handle, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(*handle, 255 * (elapsed / logo > 1 ? 1 : (elapsed / logo)));
    SDL_RenderCopy(renderer, *handle, nullptr, &dst);
}


void ProcessSplash::updateBanner(GameRenderer &renderer) {
    auto handle = Locator::TextureCache::ref().handle("logo/division");

    SDL_Rect dst;
    dst.w = handle->width();
    dst.h = handle->height();
    dst.x = logicalWidth / 2 - dst.w / 2;
    dst.y = logicalHeight / 2 + 60;

    SDL_SetTextureBlendMode(*handle, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(*handle, 255 * (elapsed / logo > 1 ? 1 : (elapsed / logo)));
    SDL_RenderCopy(renderer, *handle, nullptr, &dst);
}


void ProcessSplash::update(delta_type delta, void *data) {
    auto &renderer = *static_cast<GameRenderer *>(data);

    elapsed += delta;

    if(elapsed >= duration) {
        Locator::Dispatcher::ref().enqueue<SceneEvent>(SceneType::MENU_PAGE);
        succeed();
    } else {
        updateRainbow(renderer);
        updateGamee(renderer);
        updateBanner(renderer);
    }
}


}
