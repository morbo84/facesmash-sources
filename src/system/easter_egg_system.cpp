#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../factory/play_factory.h"
#include "../locator/locator.hpp"
#include "easter_egg_system.h"


namespace gamee {


EasterEggSystem::EasterEggSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<ActivateEasterEggEvent>(this);
}


EasterEggSystem::~EasterEggSystem() {
    Locator::Dispatcher::ref().disconnect<ActivateEasterEggEvent>(this);
}


void EasterEggSystem::receive(const ActivateEasterEggEvent &) noexcept {
    dirty = true;
}


void EasterEggSystem::update(Registry &registry, PlayFactory &factory) {
    if(dirty) {
        if(unlock) {
            --unlock;
        } else {
            factory.spawnFaceBottom(registry, 0, 0, bag.get());
            registry.reset<Destroyable>();
        }

        dirty = false;
    }
}


}
