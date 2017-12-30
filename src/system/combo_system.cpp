#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "combo_system.h"


namespace gamee {


ComboSystem::ComboSystem()
    : remaining{interval},
      current{0_ui8}
{
    Locator::Dispatcher::ref().connect<SmashEvent>(this);
}


ComboSystem::~ComboSystem() {
    Locator::Dispatcher::ref().disconnect<SmashEvent>(this);
}


void ComboSystem::receive(const SmashEvent &event) noexcept {
    current = event.miss ? 0_ui8 : current;
    current = event.smash ? (current+1) : current;
    remaining = interval;
}


void ComboSystem::update(Registry &registry, delta_type delta) {
    // TODO
}


}
