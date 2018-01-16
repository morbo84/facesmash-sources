#include <algorithm>
#include <type_traits>
#include <SDL_pixels.h>
#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "timer_system.h"


namespace gamee {


void TimerSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<LetsPlay>()) {
        auto &textureCache = Locator::TextureCache::ref();
        auto &gameTimer = registry.get<GameTimer>();
        auto symEmptyHandle = textureCache.handle("label/debug/small/ ");

        gameTimer.remaining -= std::min(gameTimer.remaining, delta);

        const int last = std::extent<decltype(GameTimer::entities)>::value;
        auto remaining = gameTimer.remaining / 1000;
        const int offset = numOfDigits(remaining);

        for(auto i = offset; i < last; ++i) {
            registry.accomodate<HUD>(gameTimer.entities[i], symEmptyHandle, symEmptyHandle->width(), symEmptyHandle->height(), symEmptyHandle->width(), symEmptyHandle->height());
        }

        for(auto i = offset; i > 0; --i) {
            auto handle = toLabelDebugSmall(remaining % 10);
            registry.accomodate<HUD>(gameTimer.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
            remaining /= 10;
        }

        if(0 == gameTimer.remaining) {
            Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::GAME_OVER);
        }
    }
}


}
