#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "../common/types.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../game/game_renderer.h"
#include "../math/math.hpp"
#include "rendering_system.h"


namespace gamee {


void RenderingSystem::update(Registry &registry, GameRenderer &renderer) {
    registry.sort<Renderable>([](const auto &lhs, const auto &rhs) {
        return (lhs.z == rhs.z) ? (&lhs < &rhs) : (lhs.z < rhs.z);
    });

    const auto offset = transformToPosition(registry, registry.attachee<Camera>(), registry.get<Transform>(registry.attachee<Camera>()));
    auto view = registry.persistent<Transform, Renderable, Sprite>();
    const SDL_Rect screen = logicalScreen;

    view.sort<Renderable>();

    view.each([&](auto entity, const auto &transform, const auto &renderable, const auto &sprite) {
        // move objects according to camera position
        const auto position = transformToPosition(registry, entity, transform) - offset;

        SDL_Rect src;
        SDL_Rect dst;

        const auto pos = sprite.frame % sprite.line;

        src.w = sprite.width;
        src.h = sprite.height;
        src.x = sprite.x + src.w * pos;
        src.y = sprite.y + src.h * (sprite.frame - pos) / sprite.line;

        dst.w = sprite.w;
        dst.h = sprite.h;
        dst.x = position.x;
        dst.y = position.y;

        if(SDL_HasIntersection(&screen, &dst)) {
            SDL_Texture *texture = sprite.handle.get();
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
        }
    });

#if DEBUG
    if(registry.has<LetsPlay>()) {
        const auto &play = registry.get<LetsPlay>();
        SDL_SetRenderDrawColor(renderer, 255_ui8, 0_ui8, 0_ui8, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(renderer, &play.smashArea);
    }
#endif // DEBUG
}


}
