#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../scene/scene_game.h"
#include "../scene/scene_menu.h"
#include "../scene/scene_null.h"
#include "../scene/scene_splash.h"
#include "game_renderer.h"
#include "game_loop.h"


namespace gamee {


void GameLoop::setup() {
    // TODO load resources
    // (at least the ones necessary for the first scene)
}


void GameLoop::shutdown() {
    // TODO unload resources

    current.reset();
}


void GameLoop::update(GameRenderer &renderer, delta_type delta) {
    renderer.clear();
    current->update(renderer, delta);
    renderer.present();
}


GameLoop::GameLoop(): current{std::make_unique<SceneNull>()} {
    Locator::Dispatcher::ref().connect<SceneEvent>(this);
    Locator::Dispatcher::ref().enqueue<SceneEvent>(SceneEvent::Type::SPLASH);
}


GameLoop::~GameLoop() {
    Locator::Dispatcher::ref().disconnect<SceneEvent>(this);
}


void GameLoop::receive(const SceneEvent &event) noexcept {
    if(current) {
        current->leaving();
    }

    switch(event.type) {
    case SceneEvent::Type::GAME:
        current = std::make_unique<SceneGame>();
        break;
    case SceneEvent::Type::MENU:
        current = std::make_unique<SceneMenu>();
        break;
    case SceneEvent::Type::SPLASH:
        current = std::make_unique<SceneSplash>();
        break;
    default:
        current = std::make_unique<SceneNull>();
    }

    current->entering();
}


/*
static constexpr delta_type msPerUpdate = 20;

private:
    // tracking value
    delta_type accumulator{0_ui32};

void GameLoop::loop(GameRenderer &renderer, delta_type elapsed) noexcept {
    // load required assets if any
    assetsSystem.update(renderer);

    // update current scene if needed and check if stanzas have been reached
    sceneSystem.update();
    stanzaSystem.update();

    // manage user inputs of any type
    userInputSystem.update();
    moodSystem.update();

    // sum what remains from the previous step
    accumulator += elapsed;

    // invoke systems at 50 fps (but for rendering and few other systems)
    while(accumulator >= msPerUpdate) {
        // update the world all at once
        commandSystem.update(msPerUpdate);
        physicsSystem.update(msPerUpdate);
        // update finite state machines, then reset or advance animations
        fsmPlayingCharacterSystem.update(msPerUpdate);
        fsmGuardSystem.update(msPerUpdate);
        fsmRunnerSystem.update(msPerUpdate);
        // update camera position
        cameraSystem.update(elapsed);
        // consume a token
        accumulator -= msPerUpdate;
    }

    // invoke systems that require to perform a linear interpolation
    UInt32 step = accumulator / msPerUpdate;
    // lerp_system.update(step)
    (void)step;

    // update animation steps
    animationSystem.update(elapsed);

    // switch to the targeted context
    renderer.context(GameRenderer::Context::TARGETED);

    // perform rendering of scene
    renderingSystem.update(renderer);

    // switch back to the default context and copy over the scene
    renderer.context(GameRenderer::Context::DEFAULT);
    renderer.copy();

    // print out hud information
    hudSystem.update(renderer, elapsed);

    // present content
    renderer.present();
}
*/


}
