#include <memory>
#include <utility>
#include <SDL_render.h>
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../factory/game_factory.h"
#include "../locator/locator.hpp"
#include "../resource/assets.h"
#include "game_renderer.h"
#include "game_loop.h"


namespace gamee {


void GameLoop::init(GameRenderer &renderer) {
    // load all the resources at once (as long as it works, why not?)
    // then create all the panels, so as to have them always available

    loadDefaultFont();
    loadGameStuff(renderer);
    loadTargetTextures(renderer);

    createSplashScreen(registry);
    createBackgroundTopPanel(registry);
    createBackgroundBottomPanel(registry);
    createMenuTopPanel(registry);
    createMenuBottomPanel(registry);
    createCreditsPanel(registry);
    createSupportPanel(registry);
    createSettingsPanel(registry);
    createAchievementsPanel(registry);
    createTutorialTopPanel(registry);
    createTutorialBottomPanel(registry);
    createGameTopPanel(registry);
    createGameBottomPanel(registry);
    createGameOverPanel(registry);
    createTrainingTopPanel(registry);
    createTrainingBottomPanel(registry);
    createCamera(registry);

#ifdef CAMERA_FRAME_AVAILABLE
    loadCameraFrame(renderer);
    createCameraFrame(registry);
#endif // CAMERA_FRAME_AVAILABLE

#ifdef DEBUG
    loadDebugFont();
    loadDebugStuff(renderer);

    createDebugHUD(registry);
    createSmashButtons(registry);
#endif // DEBUG

    // init systems explicitly if required
    recordingSystem.init(renderer);

    // request immediately a transition to the main menu from the splash screen
    Locator::Dispatcher::ref().trigger<SceneChangeEvent>(SceneType::SPLASH_SCREEN);
}



void GameLoop::close() {
    // reset cache (and avoid crashes)
    Locator::TextureCache::ref().clear();
    Locator::TTFFontCache::ref().clear();
}


void GameLoop::update(GameRenderer &renderer, delta_type delta) {
    // do the best to record if required and then render everything
    recordingSystem.update(renderer, delta, [&, this](bool isRecording) {
        // sum what remains from the previous step
        accumulator50FPS += delta;
        accumulator25FPS += delta;

        sceneSystem.update(registry, delta);
        destroyLaterSystem.update(registry, delta);

        uiButtonSystem.update(registry);
        smashButtonSystem.update(registry);

        itemSystem.update(registry, factory, delta);
        faceSmashSystem.update(registry, factory);
        rewardSystem.update(registry);

        // invoke systems at 50 fps
        while(accumulator50FPS >= msPerUpdate50FPS) {
            movementSystem.update(registry, msPerUpdate50FPS);
            animationSystem.update(registry, msPerUpdate50FPS);
            // consume a token
            accumulator50FPS -= msPerUpdate50FPS;
        }

        // enable or disable the frame system
        bool canAcquire = !isRecording;

        // invoke systems at 25 fps
        while(accumulator25FPS >= msPerUpdate25FPS) {
            scoreSystem.update(registry);
            timerSystem.update(registry, msPerUpdate25FPS);
            cameraSystem.update(registry, msPerUpdate25FPS);

            // camera stream set at 25fps and we avoid acquiring a frame in case
            // we are recording the current scene to avoid stressing the cpu
            if(canAcquire) {
                frameSystem.update(registry);
                canAcquire = !canAcquire;
            }

            // consume a token
            accumulator25FPS -= msPerUpdate25FPS;
        }

        theGameSystem.update(registry, factory);
        trainingSystem.update(registry, factory);

        // update debug information (no fixed step here, thanks)
        debugSystem.update(registry, delta);

        // finally, render everything on screen
        renderingSystem.update(registry, renderer);
    });
}


}
