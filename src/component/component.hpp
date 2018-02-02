#ifndef FACE_SMASH_COMPONENT_COMPONENT_HPP
#define FACE_SMASH_COMPONENT_COMPONENT_HPP


#include <SDL_rect.h>
#include "../common/constants.h"
#include "../common/ease.h"
#include "../common/types.h"
#include "../resource/texture_resource.h"


namespace gamee {


struct DebugInfo final {
    entity_type time[4];
    entity_type fps[2];
    float average{0.f};
};


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
    Uint8 frames{1_ui8};
    Uint8 line{1_ui8};
    Uint8 frame{0_ui8};
};


struct Renderable final {
    float angle{0.f};
    int z{127};
    int alpha{255};
};


struct Transform final {
    entity_type parent;
    float x;
    float y;
};


struct Camera final {};


struct CameraShake final {
    delta_type remaining{250_ui32};
    Uint8 shake{4_ui8};
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
    delta_type duration;
    delta_type elapsed{0_ui32};
    bool repeat{true};
};


struct RotationAnimation final {
    float from;
    float to;
    delta_type duration;
    delta_type elapsed{0_ui32};
    bool repeat{true};
};


struct FadeAnimation final {
    using ease_type = float(*)(float, float, float, float);
    int from;
    int to;
    delta_type duration;
    delta_type elapsed{0_ui32};
    ease_type ease{&linear};
};


struct HorizontalAnimation final {
    using ease_type = float(*)(float, float, float, float);
    int from;
    int to;
    delta_type duration;
    delta_type elapsed{0_ui32};
    ease_type ease{&linear};
};


struct VerticalAnimation final {
    using ease_type = float(*)(float, float, float, float);
    int from;
    int to;
    delta_type duration;
    delta_type elapsed{0_ui32};
    ease_type ease{&linear};
};


struct SizeAnimation final {
    using ease_type = float(*)(float, float, float, float);
    int fromW;
    int fromH;
    int toW;
    int toH;
    delta_type duration;
    delta_type elapsed{0_ui32};
    ease_type ease{&linear};
};


struct UIButton final {
    bool enabled;
    UIAction action;
};


struct Movement final {
    float gravity;
    float velX;
    float velY;
    float maxVelY{1.6f};
    float mod{1.f};
};


struct Face final {
    Uint8 smash;
    Uint8 miss;
    FaceType type;
};


struct Item final {
    ItemType type;
};


struct Destroyable final {};


struct DestroyLater final {
    delta_type remaining{0_ui32};
};


struct ExpiringContent final {};


struct GameTimer final {
    entity_type entities[2];
    delta_type remaining{1999_ui32};
    bool enabled{false};
};


struct PlayerScore final {
    entity_type entities[5];
    int score{0};
    int current{0};
    int hitAngry{0};
    int hitDisgusted{0};
    int hitHappy{0};
    int hitSurprised{0};
    int hitFearful{0};
    int hitSad{0};
};


struct Reward final {
    bool dirty{false};
};


struct LetsPlay final {
    // time steps
    bool remaining55000{false};
    bool remaining50000{false};
    bool remaining40000{false};
    bool remaining30000{false};
    bool remaining15000{false};
    bool remaining7500{false};
    // score steps
    int nextScoreStep{2500};
};


struct Panel final {
    int w;
    int h;
    PanelType type;
};


struct CameraFrame final {
    bool acquire{false};
};


}


#endif // FACE_SMASH_COMPONENT_COMPONENT_HPP
