#include <SDL_timer.h>
#include <type_traits>
#include "../common/util.h"
#include "../component/component.hpp"
#include "debug_system.h"


namespace gamee {


void DebugSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<TimeDebug>()) {
        auto &timeDebug = registry.get<TimeDebug>();

        timeDebug.average = (timeDebug.average * .9f) + (delta * .1f);
        int time = 10 * timeDebug.average;

        for(auto i = std::extent<decltype(TimeDebug::entities)>::value; i > 0u; --i) {
            auto handle = toStrDebug(time % 10);
            registry.accomodate<HUD>(timeDebug.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
            time /= 10;
        }

        if(registry.has<FPSDebug>()) {
            const auto &fpsDebug = registry.get<FPSDebug>();
            int fps = timeDebug.average ? (1000.f / timeDebug.average) : 0;

            for(auto i = std::extent<decltype(FPSDebug::entities)>::value; i > 0u; --i) {
                auto handle = toStrDebug(fps % 10);
                registry.accomodate<HUD>(fpsDebug.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
                fps /= 10;
            }
        }
    }
}



}
