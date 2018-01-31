#include <type_traits>
#include "../common/util.h"
#include "../component/component.hpp"
#include "debug_system.h"


namespace gamee {


void DebugSystem::update(Registry &registry, delta_type delta) {
    registry.view<DebugInfo>().each([&](auto, auto &debug) {
        debug.average = (debug.average * .9f) + (delta * .1f);

        int time = 10 * debug.average;

        for(auto i = std::extent<decltype(DebugInfo::time)>::value; i > 0u; --i) {
            auto handle = toStrDebug(time % 10);
            registry.accomodate<HUD>(debug.time[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
            time /= 10;
        }

        int fps = debug.average ? (1000.f / debug.average) : 0;

        for(auto i = std::extent<decltype(DebugInfo::fps)>::value; i > 0u; --i) {
            auto handle = toStrDebug(fps % 10);
            registry.accomodate<HUD>(debug.fps[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
            fps /= 10;
        }
    });
}



}
