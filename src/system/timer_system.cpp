#include <algorithm>
#include <type_traits>
#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "timer_system.h"


namespace gamee {


void TimerSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<GameTimer>()) {
        auto &gameTimer = registry.get<GameTimer>();

        if(gameTimer.enabled) {
            gameTimer.remaining -= std::min(gameTimer.remaining, delta);

            const int last = std::extent<decltype(GameTimer::entities)>::value;
            auto remaining = gameTimer.remaining / 1000;
            const int offset = last - numOfDigits(remaining);

            for(auto i = 0; i < offset; i++) {
                const auto sym0Handle = toStrHud(0);
                registry.accomodate<HUD>(gameTimer.entities[i], sym0Handle, sym0Handle->width(), sym0Handle->height(), sym0Handle->width(), sym0Handle->height());
            }

            for(auto i = last; i > offset; --i) {
                auto handle = toStrHud(remaining % 10);
                registry.accomodate<HUD>(gameTimer.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
                remaining /= 10;
            }

            if(0 == gameTimer.remaining) {
                Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::GAME_OVER);
            }
        }
    }
}


}
