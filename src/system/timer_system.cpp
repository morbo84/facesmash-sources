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
    if(!registry.empty<Timer>()) {
        auto &timer = *registry.raw<Timer>();

        if(timer.enabled) {
            timer.remaining -= std::min(timer.remaining, delta);

            registry.view<TimerObserver>().each([&](auto, auto &observer) {
                const int last = std::extent<decltype(TimerObserver::entities)>::value;
                auto remaining = timer.remaining / 1000;
                const int offset = last - numOfDigits(remaining);

                for(auto i = 0; i < offset; i++) {
                    const auto sym0Handle = toStrSmallHandle(0);
                    registry.assign_or_replace<HUD>(observer.entities[i], sym0Handle, sym0Handle->width(), sym0Handle->height(), sym0Handle->width(), sym0Handle->height());
                }

                for(auto i = last; i > offset; --i) {
                    auto handle = toStrSmallHandle(remaining % 10);
                    registry.assign_or_replace<HUD>(observer.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
                    remaining /= 10;
                }
            });

            if(!timer.remaining) {
                timer.enabled = false;
                Locator::Dispatcher::ref().enqueue<TimeIsOverEvent>();
            }
        }
    }
}


}
