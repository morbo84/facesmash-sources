#include <cassert>
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../scene/scene_challenge.h"
#include "../scene/scene_timer.h"
#include "../scene/scene_training.h"
#include "../scene/scene_menu.h"
#include "../scene/scene_null.h"
#include "../scene/scene_splash.h"
#include "game_renderer.h"
#include "game_loop.h"


namespace gamee {


void GameLoop::update(GameRenderer &renderer, delta_type delta) {
    renderer.clear();

    if(dirty) {
        current->leaving();
        current.swap(next);
        next.reset();
        current->entering();
        dirty = false;
    }

    current->update(renderer, delta);
    renderer.present();
}


GameLoop::GameLoop()
    : current{std::make_unique<SceneNull>()},
      next{nullptr},
      dirty{false}
{
    Locator::Dispatcher::ref().connect<SceneEvent>(this);
    Locator::Dispatcher::ref().enqueue<SceneEvent>(SceneType::SPLASH_SCREEN);
}


GameLoop::~GameLoop() {
    Locator::Dispatcher::ref().disconnect<SceneEvent>(this);
}


void GameLoop::receive(const SceneEvent &event) noexcept {
    switch(event.type) {
    case SceneType::GAME_TRAINING:
        next = std::make_unique<SceneTraining>();
        break;
    case SceneType::GAME_TIMER:
        next = std::make_unique<SceneTimer>();
        break;
    case SceneType::GAME_CHALLENGE:
        next = std::make_unique<SceneChallenge>();
        break;
    case SceneType::MENU_PAGE:
        next = std::make_unique<SceneMenu>();
        break;
    case SceneType::SPLASH_SCREEN:
        next = std::make_unique<SceneSplash>();
        break;
    default:
        assert(false);
        (void)0;
    }

    dirty = true;
}


}
