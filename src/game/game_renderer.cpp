#include <SDL_rect.h>
#include <SDL_hints.h>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "game_renderer.h"


namespace gamee {


GameRenderer::GameRenderer()
    : window{nullptr}, renderer{nullptr}
{
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    int init = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);

    if(0 == init) {
        UInt32 format = SDL_GetWindowPixelFormat(window);

        if(format == SDL_PIXELFORMAT_UNKNOWN) {
            format = SDL_PIXELFORMAT_RGBA8888;
        }

        SDL_RenderSetLogicalSize(renderer, logicalWidth, logicalHeight);
    }
}


GameRenderer::~GameRenderer() {
    if(renderer) { SDL_DestroyRenderer(renderer); }
    if(window) { SDL_DestroyWindow(window); }
}


void GameRenderer::present() noexcept {
    SDL_RenderPresent(renderer);
}


void GameRenderer::clear() noexcept {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}


void GameRenderer::target(SDL_Texture *texture) noexcept {
    SDL_SetRenderTarget(renderer, texture);
}


void GameRenderer::reset() noexcept {
    SDL_SetRenderTarget(renderer, nullptr);
}


GameRenderer::operator bool() const noexcept {
    return (window && renderer);
}


GameRenderer::operator SDL_Renderer * () const noexcept {
    return renderer;
}


}
