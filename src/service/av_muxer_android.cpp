#include <SDL_render.h>
#include "av_muxer_android.h"


namespace gamee {


void AvMuxerAndroid::start(int width, int height) {
    // TODO
}


void AvMuxerAndroid::frame(std::unique_ptr<FrameTexture> frame, delta_type delta) {
    SDL_Texture *texture = *frame;

    // TODO
    (void)texture;
    (void)delta;
}


void AvMuxerAndroid::stop() {
    // TODO
}


bool AvMuxerAndroid::recording() const noexcept {
    return true; // TODO
}


}
