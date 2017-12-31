#ifndef FACE_SMASH_SCENE_TIMER_H
#define FACE_SMASH_SCENE_TIMER_H


#include "../common/types.h"
#include "../system/combo_system.h"
#include "../system/destroy_later_system.h"
#include "../system/face_smash_system.h"
#include "../system/face_spawner_system.h"
#include "../system/fade_animation_system.h"
#include "../system/frame_system.h"
#include "../system/hud_system.h"
#include "../system/movement_system.h"
#include "../system/rendering_system.h"
#include "../system/rotation_animation_system.h"
#include "../system/score_system.h"
#include "../system/smash_button_system.h"
#include "../system/sprite_animation_system.h"
#include "scene.h"


namespace gamee {


class SceneTimer final: public Scene {
    static constexpr delta_type msPerUpdate = 20;

#ifdef DEBUG
    void addDebugStuff();
    void addSmashButtons();
#endif // DEBUG

    void addBackgroundFrame();
    void addHUDScore();

public:
    void update(GameRenderer &, delta_type) override;
    void entering() override;
    void leaving() override;

private:
    // tracking value
    delta_type accumulator{0_ui32};
    // entity-component system
    Registry registry;
    // systems
    ComboSystem comboSystem;
    DestroyLaterSystem destroyLaterSystem;
    FaceSmashSystem faceSmashSystem;
    FaceSpawnerSystem faceSpawnerSystem;
    FadeAnimationSystem fadeAnimationSystem;
    FrameSystem frameSystem;
    HudSystem hudSystem;
    MovementSystem movementSystem;
    RenderingSystem renderingSystem;
    RotationAnimationSystem rotationAnimationSystem;
    ScoreSystem scoreSystem;
    SpriteAnimationSystem spriteAnimationSystem;

#if DEBUG
    SmashButtonSystem smashButtonSystem;
#endif // DEBUG
};


}


#endif // FACE_SMASH_SCENE_TIMER_H
