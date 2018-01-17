#include <type_traits>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "hud_system.h"


namespace gamee {


#if DEBUG
void HudSystem::debug(Registry &registry, delta_type delta) {
    auto &timeDebug = registry.get<TimeDebug>();

    timeDebug.average = (timeDebug.average * .9f) + (delta * .1f);
    int time = 10 * timeDebug.average;

    for(auto i = std::extent<decltype(TimeDebug::entities)>::value; i > 0u; --i) {
        auto handle = toStrDebug(time % 10);
        registry.accomodate<HUD>(timeDebug.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
        time /= 10;
    }

    const auto &fpsDebug = registry.get<FPSDebug>();
    int fps = timeDebug.average ? (1000.f / timeDebug.average) : 0;

    for(auto i = std::extent<decltype(FPSDebug::entities)>::value; i > 0u; --i) {
        auto handle = toStrDebug(fps % 10);
        registry.accomodate<HUD>(fpsDebug.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
        fps /= 10;
    }
}
#endif // DEBUG


void HudSystem::update(Registry &registry, GameRenderer &renderer) {
    registry.sort<Renderable>([](const auto &lhs, const auto &rhs) {
        return (lhs.z == rhs.z) ? (&lhs < &rhs) : (lhs.z < rhs.z);
    });

    auto view = registry.persistent<Transform, Renderable, HUD>();
    view.sort<Renderable>();

    view.each([&](auto entity, const auto &transform, const auto &renderable, const auto &hud) {
        const auto position = transformToPosition(registry, entity, transform);

        SDL_Rect src;
        src.w = hud.width;
        src.h = hud.height;
        src.x = hud.x;
        src.y = hud.y;

        SDL_Rect dst;
        dst.w = hud.w;
        dst.h = hud.h;
        dst.x = position.x;
        dst.y = position.y;

        SDL_Texture *texture = hud.handle.get();
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(texture, renderable.alpha);
        SDL_RenderCopyEx(renderer, texture, &src, &dst, renderable.angle, nullptr, SDL_FLIP_NONE);

#if DEBUG
        if(registry.has<BoundingBox>(entity)) {
            SDL_Rect rect = registry.get<BoundingBox>(entity) * position;
            SDL_SetRenderDrawColor(renderer, 128_ui8, 128_ui8, 128_ui8, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(renderer, &rect);
        }
#endif // DEBUG
    });
}


void HudSystem::update(Registry &registry, GameRenderer &renderer, [[maybe_unused]] delta_type delta) {
#if DEBUG
    // update debug information
    debug(registry, delta);
#endif // DEBUG

    // render out hud
    update(registry, renderer);
}


}
