#ifndef FACE_SMASH_SCENE_GAME_H
#define FACE_SMASH_SCENE_GAME_H


#include "../system/face_spawner_system.h"
#include "../system/fade_animation_system.h"
#include "../system/hud_system.h"
#include "../system/path_updater_system.h"
#include "../system/rendering_system.h"
#include "../system/rotation_animation_system.h"
#include "../system/sprite_animation_system.h"
#include "../types/types.hpp"
#include "scene.h"


namespace gamee {


class SceneGame final: public Scene {
    static constexpr delta_type msPerUpdate = 20;

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
    FaceSpawnerSystem faceSpawnerSystem;
    FadeAnimationSystem fadeAnimationSystem;
    HudSystem hudSystem;
    PathUpdaterSystem pathUpdaterSystem;
    RenderingSystem renderingSystem;
    RotationAnimationSystem rotationAnimationSystem;
    SpriteAnimationSystem spriteAnimationSystem;
};


}


#endif // FACE_SMASH_SCENE_GAME_H
