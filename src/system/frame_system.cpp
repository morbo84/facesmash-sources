#include <cstring>
#include <SDL_render.h>
#include "../common/types.h"
#include "../locator/locator.hpp"
#include "frame_system.h"


namespace gamee {


void FrameSystem::update() {
    auto handle = Locator::TextureCache::ref().handle("camera/frame");

    if(handle) {
        Locator::Camera::ref().frame([&](const void *pixels, int size) {
            void *texture;
            int pitch;

            SDL_LockTexture(*handle, nullptr, &texture, &pitch);
            /**
             * We do not use the pitch returned by SDL_LockTexture because it's
             * not valid for NV21 format. See the following question on SO for
             * more details:
             *
             *     https://stackoverflow.com/questions/48079166/sdl-locktexture-returning-an-unexpected-pitch-value/48082356#48082356
             *
             * To sum up, SDL_CreateTexture creates properly a texture the size
             * of which is more or less 1.5 times width*height (because of the
             * 12 bits representation of NV21). It does it internally in a
             * branch and the changes to obtain the correct size aren't
             * propagated to the pitch, that is still equal to the width
             * (instead of 1.5 times the width).
             *
             * Probably it's correct, I don't know. Anyway, the void pointer
             * returned by SDL_LockTexture points to a properly allocated chunk
             * of memory of the correct size. However, the pitch returned cannot
             * be used to get the size of the image.
             *
             * By using the size returned by the camera service, we just work
             * around the issue and safely use the whole image as expected.
             */
            std::memcpy(texture, pixels, size);
            SDL_UnlockTexture(*handle);
        });
    }
}


}
