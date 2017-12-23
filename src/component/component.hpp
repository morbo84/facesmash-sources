#ifndef FACE_SMASH_COMPONENT_COMPONENT_HPP
#define FACE_SMASH_COMPONENT_COMPONENT_HPP


#include <SDL_rect.h>
#include "../resource/texture_resource.h"
#include "../types/types.hpp"


namespace gamee {


#if DEBUG
struct Debug final {
    entity_type fps;
    entity_type time;
    float average;
};
#endif // DEBUG


struct HUD final {
    SDLTextureHandle handle;
    UInt16 width;
    UInt16 height;
    UInt16 w;
    UInt16 h;
    UInt16 x{0_ui16};
    UInt16 y{0_ui16};
};


struct Sprite final {
    SDLTextureHandle handle;
    UInt16 width;
    UInt16 height;
    UInt16 w;
    UInt16 h;
    UInt16 x{0_ui16};
    UInt16 y{0_ui16};
    UInt8 frames{1_ui8};
    UInt8 frame{0_ui8};
    bool horizontal{true};
};


struct Renderable final {
    float angle{0.f};
    float factor{1.f};
    UInt8 alpha{255_ui8};
    bool flip{false};
};


struct Transform final {
    float x;
    float y;
    float z;
};


struct Camera final {
    float x;
    float y;
};


struct BoundingBox final: SDL_Rect {
    BoundingBox(int x, int y, int w, int h)
        : SDL_Rect{x, y, w, h}
    {}

    BoundingBox(int w, int h)
        : BoundingBox{0, 0, w, h}
    {}
};


struct SpriteAnimation final {
    UInt16 duration;
    UInt16 elapsed{0_ui16};
    bool repeat{true};
};


struct FadeAnimation final {
    UInt8 from;
    UInt8 to;
    UInt16 duration;
    UInt16 elapsed{0_ui16};
};


struct RotationAnimation final {
    float from;
    float to;
    UInt16 duration;
    UInt16 elapsed{0_ui16};
    bool repeat{true};
};


struct UIButton final {
    enum class Action: UInt8 {
        PLAY
    };

    Action action;
};


}


#endif // FACE_SMASH_COMPONENT_COMPONENT_HPP
