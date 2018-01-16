#ifndef FACE_SMASH_GAME_GAME_LOOP_H
#define FACE_SMASH_GAME_GAME_LOOP_H


#include <memory>
#include "../common/types.h"
#include "../factory/factory.h"
#include "../system/animation_system.h"
#include "../system/camera_system.h"
#include "../system/destroy_later_system.h"
#include "../system/face_smash_system.h"
#include "../system/frame_system.h"
#include "../system/hud_system.h"
#include "../system/item_system.h"
#include "../system/movement_system.h"
#include "../system/rendering_system.h"
#include "../system/reward_system.h"
#include "../system/scene_system.h"
#include "../system/score_system.h"
#include "../system/smash_button_system.h"
#include "../system/the_game_system.h"
#include "../system/timer_system.h"
#include "../system/training_system.h"
#include "../system/ui_button_system.h"
#include "game_env.h"


namespace gamee {


struct GameRenderer;


class GameLoop final: public GameEnv {
    static constexpr delta_type msPerUpdate50FPS = 20;
    static constexpr delta_type msPerUpdate20FPS = 50;

    void loadResources(GameRenderer &);

    void createSplashScreen();
    void createBackgroundTopPanel();
    void createBackgroundBottomPanel();
    void createMenuTopPanel();
    void createMenuBottomPanel();
    void createCameraFrame();
    void createCreditsPanel();
    void createTutorialTopPanel();
    void createTutorialBottomPanel();
    void createGameTopPanel();
    void createGameBottomPanel();
    void createGameOverPanel();
    void createTrainingTopPanel();
    void createTrainingBottomPanel();
    void createCamera();

#ifdef DEBUG
    void createSmashButtons();
    void createDebugStuff();
#endif // DEBUG

    void init(GameRenderer &) override;

    void close() override;

    void update(GameRenderer &, delta_type) override;

private:
    // tracking value
    delta_type accumulator50FPS{0_ui32};
    delta_type accumulator20FPS{0_ui32};
    // entity-component system
    Registry registry;
    // factory of game objects
    Factory factory;
    // systems
    AnimationSystem animationSystem;
    RewardSystem rewardSystem;
    CameraSystem cameraSystem;
    DestroyLaterSystem destroyLaterSystem;
    ItemSystem itemSystem;
    FaceSmashSystem faceSmashSystem;
    FrameSystem frameSystem;
    HudSystem hudSystem;
    MovementSystem movementSystem;
    RenderingSystem renderingSystem;
    SceneSystem sceneSystem;
    ScoreSystem scoreSystem;
#if DEBUG
    SmashButtonSystem smashButtonSystem;
#endif // DEBUG};
    TheGameSystem theGameSystem;
    TimerSystem timerSystem;
    TrainingSystem trainingSystem;
    UIButtonSystem uiButtonSystem;
};


}


#endif // FACE_SMASH_GAME_GAME_LOOP_H
