#ifndef FACE_SMASH_COMPONENT_COMPONENT_HPP
#define FACE_SMASH_COMPONENT_COMPONENT_HPP


#include <SDL_rect.h>
#include "../common/types.h"
#include "../resource/texture_resource.h"


namespace gamee {


struct TimeDebug final {
    float average;
};


struct FPSDebug final {};


struct SmashButton final {
    FaceType type;
};


struct HUD final {
    SDLTextureHandle handle;
    int width;
    int height;
    int w;
    int h;
    int x{0};
    int y{0};
};


struct Sprite final {
    SDLTextureHandle handle;
    int width;
    int height;
    int w;
    int h;
    int x{0};
    int y{0};
    UInt8 frames{1_ui8};
    UInt8 line{1_ui8};
    UInt8 frame{0_ui8};
};


struct Renderable final {
    float angle{0.f};
    int z{127};
    int alpha{255};
};


struct Transform final {
    float x;
    float y;
};


struct Camera final {};


struct BoundingBox final: SDL_Rect {
    BoundingBox(int x, int y, int w, int h)
        : SDL_Rect{x, y, w, h}
    {}

    BoundingBox(int w, int h)
        : BoundingBox{0, 0, w, h}
    {}
};


struct SpriteAnimation final {
    delta_type duration;
    delta_type elapsed{0_ui32};
    bool repeat{true};
};


struct FadeAnimation final {
    int from;
    int to;
    delta_type duration;
    delta_type elapsed{0_ui32};
};


struct RotationAnimation final {
    float from;
    float to;
    delta_type duration;
    delta_type elapsed{0_ui32};
    bool repeat{true};
};


struct UIButton final {
    enum class Action: UInt8 {
        PLAY,
        MENU
    };

    Action action;
};


struct Movement final {
    float gravity;
    float velX;
    float velY;
};


struct FaceSmash final {
    FaceType type;
    UInt8 smash{100_ui8};
    UInt8 miss{50_ui8};
};


struct DestroyLater final {
    delta_type remaining{0_ui32};
};


struct SceneChangeRequest final {
    SceneType scene;
    delta_type remaining{0_ui32};
};


struct SpawnRequest final {
    SDL_Rect target;
    int from;
    int width;
    bool horizontal;
    bool rightBottom;
    delta_type xt;
    delta_type yt;
    delta_type interval;
    delta_type elapsed{0_ui32};
};


struct GameTimer final {
    delta_type remaining;
};


struct PlayerScore final {
    int score{0};
};


struct BonusLabel final {};


}


#endif // FACE_SMASH_COMPONENT_COMPONENT_HPP
