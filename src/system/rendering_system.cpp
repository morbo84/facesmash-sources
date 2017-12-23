#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../component/component.hpp"
#include "../game/game_renderer.h"
#include "../math/math.hpp"
#include "../settings/settings.h"
#include "../types/types.hpp"
#include "rendering_system.h"


namespace gamee {


void RenderingSystem::update(Registry &registry, GameRenderer &renderer) {
    registry.sort<Transform>([](const auto &lhs, const auto &rhs) {
        return (lhs.z == rhs.z) ? (&lhs < &rhs) : (lhs.z < rhs.z);
    });

    const auto &offset = registry.get<Transform>(registry.attachee<Camera>());
    auto view = registry.persistent<Transform, Renderable, Sprite>();

    view.sort<Transform>();

    Settings settings{};
    SDL_Rect screen;
    screen.x = 0;
    screen.y = 0;
    screen.w = settings.logicalWidth();
    screen.h = settings.logicalHeight();

    view.each([&]([[maybe_unused]] auto entity, const auto &transform, const auto &renderable, const auto &sprite) {
        // move objects according to camera position
        const auto position = transform - offset;

        SDL_Rect src;
        SDL_Rect dst;

        src.w = sprite.width;
        src.h = sprite.height;
        src.x = sprite.x + (sprite.horizontal ? (src.w * sprite.frame) : 0);
        src.y = sprite.y + (sprite.horizontal ? 0 : (src.h * sprite.frame));

        dst.w = sprite.w;
        dst.h = sprite.h;
        dst.x = position.x / renderable.factor;
        dst.y = position.y;

        if(SDL_HasIntersection(&screen, &dst)) {
            SDL_Texture *texture = sprite.handle.get();
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(texture, renderable.alpha);
            SDL_RendererFlip flip = renderable.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            SDL_RenderCopyEx(renderer, texture, &src, &dst, renderable.angle, nullptr, flip);

#if DEBUG
            if(registry.has<BoundingBox>(entity)) {
                SDL_Rect rect = registry.get<BoundingBox>(entity) * position;
                SDL_SetRenderDrawColor(renderer, 128_ui8, 128_ui8, 128_ui8, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawRect(renderer, &rect);
            }
#endif // DEBUG
        }
    });

#if DEBUG
    const auto &camera = registry.get<Camera>();
    SDL_SetRenderDrawColor(renderer, 255_ui8, 255_ui8, 255_ui8, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(renderer, camera.x - 48, camera.y, camera.x + 48, camera.y);
    SDL_RenderDrawLine(renderer, camera.x - 48, camera.y - 1, camera.x + 48, camera.y - 1);
    SDL_RenderDrawLine(renderer, camera.x, camera.y - 48, camera.x, camera.y + 48);
    SDL_RenderDrawLine(renderer, camera.x - 1, camera.y - 48, camera.x - 1, camera.y + 48);
#endif // DEBUG
}


}
