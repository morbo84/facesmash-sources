#include <type_traits>
#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "debug_system.h"


namespace gamee {


DebugSystem::DebugSystem() {
    Locator::Dispatcher::ref().connect<DebugEvent>(this);
}


DebugSystem::~DebugSystem() {
    Locator::Dispatcher::ref().disconnect<DebugEvent>(this);
}


void DebugSystem::receive(const DebugEvent &event) noexcept {
    last = event.delta;
}


void DebugSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<TimeDebug>()) {
        auto &timeDebug = registry.get<TimeDebug>();

        timeDebug.average = (timeDebug.average * .9f) + (delta * .1f);
        int time = 10 * timeDebug.average;

        for(auto i = std::extent<decltype(TimeDebug::entities)>::value / 2; i > 0u; --i) {
            auto handle = toStrDebug(time % 10);
            registry.accomodate<HUD>(timeDebug.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
            time /= 10;
        }

        timeDebug.spent = (timeDebug.spent * .9f) + (last * .1f);
        int spent = 10 * timeDebug.spent;

        for(auto i = std::extent<decltype(TimeDebug::entities)>::value; i > std::extent<decltype(TimeDebug::entities)>::value / 2; --i) {
            auto handle = toStrDebug(spent % 10);
            registry.accomodate<HUD>(timeDebug.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
            spent /= 10;
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
