#ifndef FACE_SMASH_GAME_GAME_LOOP_H
#define FACE_SMASH_GAME_GAME_LOOP_H


#include <memory>
#include "../common/types.h"
#include "../system/animation_system.h"
#include "../system/bonus_system.h"
#include "../system/camera_system.h"
#include "../system/destroy_later_system.h"
#include "../system/face_modifier_system.h"
#include "../system/face_smash_system.h"
#include "../system/frame_system.h"
#include "../system/hud_system.h"
#include "../system/movement_system.h"
#include "../system/rendering_system.h"
#include "../system/scene_system.h"
#include "../system/score_system.h"
#include "../system/smash_button_system.h"
#include "../system/the_game_system.h"
#include "../system/timer_system.h"
#include "../system/ui_button_system.h"
#include "game_env.h"


namespace gamee {


struct GameRenderer;


class GameLoop final: public GameEnv {
    static constexpr delta_type msPerUpdate = 20;

    void loadResources(GameRenderer &);

    void createSplashScreen();
    void createTopPatch();
    void createBottomPatch();
    void createMenuTopPanel();
    void createMenuBottomPanel();
    void createPlayButton();
    void createCameraFrame();
    void createGameTopPanel(GameRenderer &);
    void createGameBottomPanel();
    void createGameOverPanel();
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
    delta_type accumulator{0_ui32};
    // entity-component system
    Registry registry;
    // systems
    AnimationSystem animationSystem;
    BonusSystem bonusSystem;
    CameraSystem cameraSystem;
    DestroyLaterSystem destroyLaterSystem;
    FaceModifierSystem faceModifierSystem;
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
    UIButtonSystem uiButtonSystem;
};


}


#endif // FACE_SMASH_GAME_GAME_LOOP_H
