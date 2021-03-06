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

    loadResources(renderer);
    prepareGameStrings(renderer);
    prepareCameraFrame(renderer);
    prepareOpponentFrame(renderer);
    prepareWallpaperFrames(renderer);
    prepareGameStuff(renderer);

    createExitPanel(registry);
    createTitleTopPanel(registry);
    createTitleBottomPanel(registry);
    createSplashScreenPanel(registry);
    createBackgroundTopPanel(registry);
    createBackgroundBottomPanel(registry);
    createMenuTopPanel(registry);
    createMenuBottomPanel(registry);
    createInfoPanel(registry);
    createMultiplayerPanel(registry);
    createSettingsPanel(registry);
    createTutorialTopPanel(registry);
    createTutorialBottomPanel(registry);
    createGameTopPanel(registry);
    createGameBottomPanel(registry);
    createGameOverPanel(registry);
    createMultiplayerResultsPanel(registry);
    createTrainingRightPanel(registry);
    createTrainingTopPanel(registry);
    createTrainingBottomPanel(registry);
    createInviteTrainLeft(registry);
    createInviteTrainRight(registry);
    createInviteShareLeft(registry);
    createInviteShareRight(registry);
    createInviteShareTop(registry);
    createInviteShareBottom(registry);
    createWallpaperPanel(registry);
    createStanza(registry);
    createCamera(registry);
    createCameraFrame(registry);

#ifdef DEBUG
    loadDebugResources();
    prepareDebugStrings(renderer);

    createDebugHUD(registry);
    createSmashButtons(registry);
#endif // DEBUG

    // init systems explicitly if required
    recordingSystem.init();

    // request immediately a transition to the main menu from the splash screen
    Locator::Dispatcher::ref().trigger<SceneChangeEvent>(SceneType::SPLASH_SCREEN);
}



void GameLoop::close() {
    // reset caches (and avoid crashes)
    Locator::TextureCache::ref().clear();
    Locator::TTFFontCache::ref().clear();
    Locator::AudioChunkCache::ref().clear();
    Locator::AudioMusicCache::ref().clear();
}


void GameLoop::update(GameRenderer &renderer, delta_type delta) {
    // do the best to record if required and then render everything
    recordingSystem.update(renderer, delta, [&, this]() {
        // sum what remains from the previous step
        accumulator += delta;

        // check if we are dealing with a face smash supporter
        billingSystem.update(registry);

        // dequeue incoming messages as soon as possible
        multiplayerSystem.update(registry, delta);

        audioSystem.update();
        wallpaperSystem.update(renderer);
        frameSystem.update(registry, renderer);
        sceneSystem.update(registry, delta);
        destroyLaterSystem.update(registry, delta);

        uiButtonSystem.update(registry);
        faceButtonSystem.update(registry);
        smashButtonSystem.update(registry);

        itemSystem.update(registry, spawner, delta);
        faceSmashSystem.update(registry, spawner);
        stanzaSystem.update(registry, delta);

        achievementsSystem.update(registry);
        scoreSystem.update(registry);
        timerSystem.update(registry, delta);
        cameraSystem.update(registry, delta);

        easterEggSystem.update(registry, spawner);
        theGameSystem.update(registry, spawner);
        trainingSystem.update(registry, spawner, delta);

        // invoke systems with a fixed timestep
        while(accumulator >= msPerUpdate) {
            movementSystem.update(registry, msPerUpdate);
            animationSystem.update(registry, msPerUpdate);
            // consume a token
            accumulator -= msPerUpdate;
        }

        // update debug information (no fixed step here, thanks)
        debugSystem.update(registry, delta);

        // finally, render everything on screen
        renderingSystem.update(registry, renderer);
    });
}


}
