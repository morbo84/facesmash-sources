#include <SDL_render.h>
#include "../common/types.h"
#include "../locator/locator.hpp"
#include "frame_system.h"


namespace gamee {


void FrameSystem::update() {
    auto handle = Locator::TextureCache::ref().handle("visage/frame");
    auto &cameraService = Locator::Camera::ref();

    if(cameraService.available() && cameraService.dirty() && handle) {
        SDL_UpdateTexture(*handle, nullptr, cameraService.pixels(), cameraService.pitch());
    }
}


}
