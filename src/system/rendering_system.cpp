#include <vector>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "../common/types.h"
#include "../component/component.hpp"
#include "../game/game_renderer.h"
#include "../math/math.hpp"
#include "rendering_system.h"


namespace gamee {


void RenderingSystem::update(Registry &registry, GameRenderer &renderer) {
    registry.sort<Renderable>([](const auto &lhs, const auto &rhs) {
        return (lhs.z == rhs.z) ? (&lhs < &rhs) : (lhs.z < rhs.z);
    });

    const auto &offset = registry.get<Transform>(registry.attachee<Camera>());
    auto view = registry.persistent<Transform, Renderable, Sprite>();
    const SDL_Rect screen = logicalScreen;

    view.sort<Renderable>();

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
}


}
