#include <sstream>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "hud_system.h"


namespace gamee {


#if DEBUG
void HudSystem::debug(Registry &registry, GameRenderer &renderer, delta_type delta) {
    if(registry.has<TimeDebug>()) {
        const SDL_Color fg = { 255, 255, 255, 255 };
        auto font = Locator::TTFFontCache::ref().handle("ttf/constant/36");

        auto &timeDebug = registry.get<TimeDebug>();
        auto timeEntity = registry.attachee<TimeDebug>();

        timeDebug.average = (timeDebug.average * .9f) + (delta * .1f);

        std::stringstream time;
        time.precision(3);
        time << "TIME: " << timeDebug.average;

        auto timeHandle = Locator::TextureCache::ref().temp<TTFFontTextureLoader>(time.str().c_str(), renderer, font.get(), fg);
        registry.accomodate<HUD>(timeEntity, timeHandle, timeHandle->width(), timeHandle->height(), timeHandle->width(), timeHandle->height());
        registry.accomodate<Transform>(timeEntity, 150.f, 0.f + logicalHeight - timeHandle->height());

        if(registry.has<FPSDebug>()) {
            auto fpsEntity = registry.attachee<FPSDebug>();

            std::stringstream fps;
            fps.precision(2);
            fps << "FPS: " << (1000.f / timeDebug.average);

            auto fpsHandle = Locator::TextureCache::ref().temp<TTFFontTextureLoader>(fps.str().c_str(), renderer, font.get(), fg);
            registry.accomodate<HUD>(fpsEntity, fpsHandle, fpsHandle->width(), fpsHandle->height(), fpsHandle->width(), fpsHandle->height());
            registry.accomodate<Transform>(fpsEntity, 0.f, 0.f + logicalHeight - fpsHandle->height());
        }
    }
}
#endif // DEBUG


void HudSystem::update(Registry &registry, GameRenderer &renderer) {
    registry.sort<Renderable>([](const auto &lhs, const auto &rhs) {
        return (lhs.z == rhs.z) ? (&lhs < &rhs) : (lhs.z < rhs.z);
    });

    auto view = registry.persistent<Transform, Renderable, HUD>();
    view.sort<Renderable>();

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
        SDL_RenderCopyEx(renderer, texture, &src, &dst, renderable.angle, nullptr, SDL_FLIP_NONE);

#if DEBUG
        if(registry.has<BoundingBox>(entity)) {
            SDL_Rect rect = registry.get<BoundingBox>(entity) * transform;
            SDL_SetRenderDrawColor(renderer, 128_ui8, 128_ui8, 128_ui8, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawRect(renderer, &rect);
        }
#endif // DEBUG
    });
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
