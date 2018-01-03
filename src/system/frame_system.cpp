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

        Locator::Camera::ref().pixels([&](const void *data) {
            SDL_LockTexture(*handle, nullptr, &pixels, &pitch);
            /**
             * We do not use the pitch returned by SDL_LockTexture because it's
             * not valid for NV21 format. See the following question on SO for
             * more details:
             *
             *     https://stackoverflow.com/questions/48079166/sdl-locktexture-returning-an-unexpected-pitch-value/48082356#48082356
             *
             * To sum up, SDL_CreateTexture creates properly a texture the size
             * of which is more or less 1.5 times width*height (because of the
             * 12 bits representation of NV21).
             * However, it does it internally in a branch and the changes to
             * obtain the correct size aren't propagated to the pitch, that is
             * still equal to the width (instead of 1.5 times the width).
             *
             * Because of that, the void pointer returned by SDL_LockTexture
             * points to a properly allocated chunk of memory of the correct
             * size. On the other side, the pitch returned is wrong and doesn't
             * express correctly the size of a row.
             *
             * By using the pitch returned by the camera service, we just work
             * around the issue and safely use the whole image as expected.
             */
            std::memcpy(pixels, data, cameraService.pitch() * cameraService.height());
            SDL_UnlockTexture(*handle);
        });
    }
}


}
