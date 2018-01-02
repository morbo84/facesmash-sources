#include <cstring>
#include <SDL_render.h>
#include "../common/types.h"
#include "../locator/locator.hpp"
#include "frame_system.h"


namespace gamee {


void FrameSystem::update() {
    auto handle = Locator::TextureCache::ref().handle("camera/frame");

    if(handle) {
        auto &cameraService = Locator::Camera::ref();
        void *pixels;
        int pitch;

        SDL_LockTexture(*handle, nullptr, &pixels, &pitch);

        Locator::Camera::ref().pixels([&](const void *data) {
            std::memcpy(pixels, data, pitch * cameraService.height());
        });

        SDL_UnlockTexture(*handle);
    }
}


}
