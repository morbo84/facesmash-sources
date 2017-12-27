#include <cassert>
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../scene/scene_game.h"
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
    Locator::Dispatcher::ref().enqueue<SceneEvent>(SceneEvent::Type::SPLASH);
}


GameLoop::~GameLoop() {
    Locator::Dispatcher::ref().disconnect<SceneEvent>(this);
}


void GameLoop::receive(const SceneEvent &event) noexcept {
    switch(event.type) {
    case SceneEvent::Type::GAME:
        next = std::make_unique<SceneGame>();
        break;
    case SceneEvent::Type::MENU:
        next = std::make_unique<SceneMenu>();
        break;
    case SceneEvent::Type::SPLASH:
        next = std::make_unique<SceneSplash>();
        break;
    default:
        assert(false);
        (void)0;
    }

    dirty = true;
}


}
