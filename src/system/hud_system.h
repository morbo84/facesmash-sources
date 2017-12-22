#ifndef FACE_SMASH_SYSTEM_HUD_SYSTEM_H
#define FACE_SMASH_SYSTEM_HUD_SYSTEM_H


#include "../types/types.hpp"


namespace gamee {


struct GameRenderer;


class HudSystem final {
#if DEBUG
    void debug(Registry &, GameRenderer &, delta_type);
#endif // DEBUG

    void update(Registry &, GameRenderer &);

public:
    HudSystem(TextureCache &, TTFFontCache &) noexcept;

    TextureCache & texture() const noexcept;
    void texture(TextureCache &) noexcept;

    TTFFontCache & font() const noexcept;
    void font(TTFFontCache &) noexcept;

    void update(Registry &, GameRenderer &, delta_type);

private:
    TextureCache *textureCache;
    TTFFontCache *fontCache;
};


}


#endif // FACE_SMASH_SYSTEM_HUD_SYSTEM_H
