#include "../common/util.h"
#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../factory/play_factory.h"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "easter_egg_system.h"


namespace gamee {


EasterEggSystem::EasterEggSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
}


EasterEggSystem::~EasterEggSystem() {
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


void EasterEggSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void EasterEggSystem::update(Registry &registry, PlayFactory &factory) {
    if(dirty && registry.has<EasterEgg>()) {
        auto entity = registry.attachee<EasterEgg>();

        const auto &transform = registry.get<Transform>(entity);
        const auto &box = registry.get<BoundingBox>(entity);

        auto area = transformToPosition(registry, entity, transform) * box;

        if(SDL_PointInRect(&coord, &area)) {
            auto &egg = registry.get<EasterEgg>();

            if(egg.remaining) {
                --egg.remaining;
            } else {
                factory.spawnFaceBottom(registry, 0, 0, bag.get());
                registry.reset<Face>();
            }
        }

        dirty = false;
    }
}


}
