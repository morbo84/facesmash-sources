#include <sstream>
#include <SDL_ttf.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include "../game/game_renderer.h"
#include "../component/component.hpp"
#include "../settings/settings.h"
#include "../math/math.hpp"
#include "hud_system.h"


namespace gamee {


#if DEBUG
void HudSystem::debug(Registry &registry, GameRenderer &renderer, delta_type delta) {
    auto view = registry.view<Debug>();

    SDL_Color fg = { 255, 255, 255, 255 };
    Settings settings;
    int height, width;
    UInt16 h, w;

    auto font = fontCache->handle("ttf/constant/36");

    view.each([&](auto, auto &debug) {
        debug.average = (debug.average * .9f) + (delta * .1f);

        std::stringstream time;
        time.precision(3);
        time << "TIME: " << debug.average;

        TTF_SizeText(font.get(), time.str().c_str(), &width, &height);
        h = height, w = width;

        registry.accomodate<HUD>(debug.time, textureCache->temp<TTFFontTextureLoader>(time.str().c_str(), renderer, font.get(), fg), w, h, w, h);
        registry.accomodate<Transform>(debug.time, 150.f, 0.f + settings.logicalHeight() - height);

        std::stringstream fps;
        fps.precision(2);
        fps << "FPS: " << (1000.f / debug.average);

        TTF_SizeText(font.get(), fps.str().c_str(), &width, &height);
        h = height, w = width;

        registry.accomodate<HUD>(debug.fps, textureCache->temp<TTFFontTextureLoader>(fps.str().c_str(), renderer, font.get(), fg), w, h, w, h);
        registry.accomodate<Transform>(debug.fps, 0.f, 0.f + settings.logicalHeight() - height);    });
}
#endif // DEBUG


void HudSystem::update(Registry &registry, GameRenderer &renderer) {
    registry.sort<Transform>([](const auto &lhs, const auto &rhs) {
        return (lhs.z == rhs.z) ? (&lhs < &rhs) : (lhs.z < rhs.z);
    });

    auto view = registry.persistent<Transform, Renderable, HUD>();
    view.sort<Transform>();

    view.each([&]([[maybe_unused]] auto entity, const auto &transform, const auto &renderable, const auto &hud) {
        SDL_Rect src;
        src.w = hud.width;
        src.h = hud.height;
        src.x = hud.x;
        src.y = hud.y;

        SDL_Rect dst;
        dst.w = hud.w;
        dst.h = hud.h;
        dst.x = transform.x;
        dst.y = transform.y;

        SDL_Texture *texture = hud.handle.get();
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(texture, renderable.alpha);
        SDL_RendererFlip flip = renderable.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(renderer, texture, &src, &dst, renderable.angle, nullptr, flip);

#if DEBUG
        if(registry.has<BoundingBox>(entity)) {
            SDL_Rect rect = registry.get<BoundingBox>(entity) * transform;
            SDL_SetRenderDrawColor(renderer, 128_ui8, 128_ui8, 128_ui8, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(renderer, &rect);
        }
#endif // DEBUG
    });
}


HudSystem::HudSystem(TextureCache &textureCache, TTFFontCache &fontCache) noexcept
    : textureCache{&textureCache}, fontCache{&fontCache}
{}


TextureCache & HudSystem::texture() const noexcept {
    return *textureCache;
}


void HudSystem::texture(TextureCache &cache) noexcept {
    textureCache = &cache;
}


TTFFontCache & HudSystem::font() const noexcept {
    return *fontCache;
}


void HudSystem::font(TTFFontCache &cache) noexcept {
    fontCache = &cache;
}


void HudSystem::update(Registry &registry, GameRenderer &renderer, [[maybe_unused]] delta_type delta) {
#if DEBUG
    // update debug information
    debug(registry, renderer, delta);
#endif // DEBUG

    // render out hud
    update(registry, renderer);
}


}
