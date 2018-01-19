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

    loadDefaultFont(renderer);
    loadGameStuff(renderer);

    createSplashScreen(registry);
    createBackgroundTopPanel(registry);
    createBackgroundBottomPanel(registry);
    createMenuTopPanel(registry);
    createMenuBottomPanel(registry);
    createCreditsPanel(registry);
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
    loadDebugFont(renderer);
    createDebugHUD(registry);
    createSmashButtons(registry);
#endif // DEBUG

    // request immediately a transition to the main menu from the splash screen
    Locator::Dispatcher::ref().trigger<SceneChangeEvent>(SceneType::SPLASH_SCREEN);
}



void GameLoop::close() {
    // reset cache (and avoid crashes)
    Locator::TextureCache::ref().clear();
    Locator::TTFFontCache::ref().clear();
}


void GameLoop::tick(GameRenderer &renderer, delta_type delta) {
    // sum what remains from the previous step
    accumulator50FPS += delta;
    accumulator20FPS += delta;

    // do the best to invoke systems at 60 fps

    destroyLaterSystem.update(registry, delta);
    uiButtonSystem.update(registry);
    smashButtonSystem.update(registry);

    itemSystem.update(registry, factory, delta);
    faceSmashSystem.update(registry, factory);
    rewardSystem.update(registry);

    // invoke systems at 50 fps
    while(accumulator50FPS >= msPerUpdate50FPS) {
        movementSystem.update(registry, msPerUpdate50FPS);
        frameSystem.update(registry);
        // consume a token
        accumulator50FPS -= msPerUpdate50FPS;
    }

    // invoke systems at 20 fps
    while(accumulator20FPS >= msPerUpdate20FPS) {
        scoreSystem.update(registry);
        timerSystem.update(registry, msPerUpdate20FPS);
        cameraSystem.update(registry, msPerUpdate20FPS);
        // consume a token
        accumulator20FPS -= msPerUpdate20FPS;
    }

    theGameSystem.update(registry, factory);
    trainingSystem.update(registry, factory, delta);
    animationSystem.update(registry, delta);

    renderer.clear();

    renderingSystem.update(registry, renderer);
    hudSystem.update(registry, renderer);

    renderer.present();

    // approximate remaining time and update debug information
    debugSystem.update(registry, delta);

    // update the scene for the next tick (if required)
    // this way recording starts correctly from frame 0
    sceneSystem.update(registry, delta);
}


void GameLoop::update(GameRenderer &renderer, delta_type delta) {
    auto &avMuxer = Locator::AvMuxer::ref();

    if(avMuxer.recording()) {
        constexpr auto format = SDL_PIXELFORMAT_RGBA32;
        constexpr auto size = logicalWidth * logicalHeight * SDL_BYTESPERPIXEL(format);
        constexpr auto pitch = logicalWidth * SDL_BYTESPERPIXEL(format);

        auto texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);

        renderer.target(texture);
        tick(renderer, delta);

        std::unique_ptr<unsigned char[]> frame{new unsigned char[size]};
        SDL_RenderReadPixels(renderer, nullptr, format, frame.get(), pitch);
        avMuxer.frame(std::move(frame), delta);

        renderer.reset();
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_DestroyTexture(texture);
    } else {
        tick(renderer, delta);
    }
}


}
