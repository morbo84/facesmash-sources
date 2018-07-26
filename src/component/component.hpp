#ifndef FACE_SMASH_COMPONENT_COMPONENT_HPP
#define FACE_SMASH_COMPONENT_COMPONENT_HPP


#include <SDL_rect.h>
#include "../common/constants.h"
#include "../common/ease.h"
#include "../common/types.h"
#include "../resource/texture_resource.h"


namespace gamee {


struct FaceSmashSupporter { /* Yay!! */ };


struct DebugInfo {
    entity_type time[4];
    entity_type fps[2];
    float average{0.f};
};


struct SmashButton {
    FaceType type;
};


struct FaceButton {
    bool enabled;
    FaceType type;
    int w{0};
    int h{0};
};


struct HUD {
    SDLTextureHandle handle;
    int width;
    int height;
    int w;
    int h;
    int x{0};
    int y{0};
};


struct Sprite: HUD {
    Uint8 frames{1_ui8};
    Uint8 cols{1_ui8};
    Uint8 frame{0_ui8};
};


struct Renderable {
    float angle{0.f};
    int z{127};
    int alpha{255};
    bool flip{false};
};


struct Transform {
    entity_type parent;
    float x;
    float y;
};


struct Camera {
    Uint8 shake{8_ui8};
    delta_type remaining{0_ui32};
};


struct BoundingBox final: SDL_Rect {
    BoundingBox(int x, int y, int w, int h)
        : SDL_Rect{x, y, w, h}
    {}

    BoundingBox(int w, int h)
        : BoundingBox{0, 0, w, h}
    {}
};


struct SpriteAnimation {
    delta_type duration;
    delta_type elapsed{0_ui32};
    bool repeat{true};
};


struct PulseAnimation {
    float angle;
    float length;
    float squeeze;
    delta_type duration;
    delta_type elapsed{0_ui32};
    bool repeat{true};
};


struct RotationAnimation {
    using ease_type = float(*)(float, float, float, float);
    float from;
    float to;
    delta_type duration;
    delta_type elapsed{0_ui32};
    bool repeat{true};
    ease_type ease{&linear};
};


struct FadeAnimation {
    using ease_type = float(*)(float, float, float, float);
    int from;
    int to;
    delta_type duration;
    delta_type elapsed{0_ui32};
    ease_type ease{&linear};
};


// it's useful to have different types, but after all they are identical :-)
struct HorizontalAnimation: FadeAnimation {};
struct VerticalAnimation: FadeAnimation {};


struct SizeAnimation {
    using ease_type = float(*)(float, float, float, float);
    int fromW;
    int fromH;
    int toW;
    int toH;
    delta_type duration;
    delta_type elapsed{0_ui32};
    ease_type ease{&linear};
};


struct InputReceiver {};


struct UIButton {
    bool enabled;
    UIAction action;
    bool popup{false};
    int w{0};
    int h{0};
};


struct Movement {
    float gravity;
    float velX;
    float velY;
    float maxVelY{1.6f};
    float mod{1.f};
};


struct Face {
    Uint16 smash;
    Uint16 miss;
    FaceType type;
    bool bonus{false};
};


struct Item {
    ItemType type;
};


struct Destroyable {};


struct DestroyLater {
    delta_type remaining{0_ui32};
};


struct ExpiringContent {};


struct Timer {
    delta_type remaining{0_ui32};
    bool enabled{true};
};


struct TimerObserver {
    entity_type entities[2];
};


struct PlayerScore {
    int score{0};
    int hitAngry{0};
    int hitDisgusted{0};
    int hitHappy{0};
    int hitSurprised{0};
    int hitFearful{0};
    int hitSad{0};
    int combo2x{0};
    int combo3x{0};
    int combo4x{0};
    int combo5x{0};
};


struct PlayerScoreObserver {
    entity_type entities[5];
};


struct LetsPlay {};
struct LetsTrain {};


struct Panel {
    int w;
    int h;
    PanelType type;
};


struct CameraFrame {
    bool acquire{false};
};


struct FaceSmashMessage {};
struct ItemMessage {};
struct VerticalProgressBar {};


struct Stanza final: SDL_Rect {
    Stanza (int x, int y, int w, int h, StanzaType type)
        : SDL_Rect{x, y, w, h}, type{type}
    {}

    StanzaType type;
    bool enabled{false};
};


}


#endif // FACE_SMASH_COMPONENT_COMPONENT_HPP
