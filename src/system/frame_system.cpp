#include <SDL_render.h>
#include "../common/types.h"
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


void FrameSystem::update() {
    auto handle = Locator::TextureCache::ref().handle("visage/frame");
    auto &cameraService = Locator::Camera::ref();

    if(pending && handle && cameraService.available()) {
        SDL_UpdateTexture(*handle, nullptr, cameraService.pixels(), cameraService.pitch());
        pending = false;
    }
}


}
