#ifndef FACE_SMASH_GAME_GAME_LOOP_H
#define FACE_SMASH_GAME_GAME_LOOP_H


#include <memory>

#include "../common/types.h"
#include "../factory/spawner.h"
#include "../system/achievements_system.h"
#include "../system/animation_system.h"
#include "../system/audio_system.h"
#include "../system/billing_system.h"
#include "../system/debug_system.h"
#include "../system/camera_system.h"
#include "../system/destroy_later_system.h"
#include "../system/easter_egg_system.h"
#include "../system/face_button_system.h"
#include "../system/face_smash_system.h"
#include "../system/frame_system.h"
#include "../system/item_system.h"
#include "../system/movement_system.h"
#include "../system/multiplayer_system.h"
#include "../system/recording_system.h"
#include "../system/rendering_system.h"
#include "../system/scene_system.h"
#include "../system/score_system.h"
#include "../system/smash_button_system.h"
#include "../system/stanza_system.h"
#include "../system/the_game_system.h"
#include "../system/timer_system.h"
#include "../system/training_system.h"
#include "../system/ui_button_system.h"
#include "game_env.h"


namespace gamee {


struct GameRenderer;


class GameLoop final: public GameEnv {
    static constexpr delta_type msPerUpdate = 10;

    void init(GameRenderer &) override;
    void close() override;

    void update(GameRenderer &, delta_type) override;

private:
    // tracking value
    delta_type accumulator{0_ui32};
    // entity-component system
    Registry registry;
    // factory of game objects
    Spawner spawner;
    // systems
    AchievementsSystem achievementsSystem;
    AnimationSystem animationSystem;
    AudioSystem audioSystem;
    BillingSystem billingSystem;
    DebugSystem debugSystem;
    CameraSystem cameraSystem;
    DestroyLaterSystem destroyLaterSystem;
    EasterEggSystem easterEggSystem;
    FaceButtonSystem faceButtonSystem;
    ItemSystem itemSystem;
    FaceSmashSystem faceSmashSystem;
    FrameSystem frameSystem;
    MovementSystem movementSystem;
    MultiplayerSystem multiplayerSystem;
    RecordingSystem recordingSystem;
    RenderingSystem renderingSystem;
    SceneSystem sceneSystem;
    ScoreSystem scoreSystem;
    SmashButtonSystem smashButtonSystem;
    StanzaSystem stanzaSystem;
    TheGameSystem theGameSystem;
    TimerSystem timerSystem;
    TrainingSystem trainingSystem;
    UIButtonSystem uiButtonSystem;
};


}


#endif // FACE_SMASH_GAME_GAME_LOOP_H
