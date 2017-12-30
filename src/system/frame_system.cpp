#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "frame_system.h"


namespace gamee {


FrameSystem::FrameSystem(): pending{false} {
    Locator::Dispatcher::ref().connect<FrameEvent>(this);
}


FrameSystem::~FrameSystem() {
    Locator::Dispatcher::ref().disconnect<FrameEvent>(this);
}


void FrameSystem::receive(const FrameEvent &) noexcept {
    pending = true;
}


void FrameSystem::update(Registry &registry) {
    if(pending) {
        // TODO

        pending = false;
    }
}


}
