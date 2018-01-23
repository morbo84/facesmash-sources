#include <SDL_render.h>
#include "av_muxer_android.h"


namespace gamee {


void AvMuxerAndroid::start(int width, int height) {
    // TODO
}


void AvMuxerAndroid::frame(const unsigned char *pixels, delta_type delta) {
    // TODO
    (void)pixels;
    (void)delta;
}


void AvMuxerAndroid::stop() {
    // TODO
}


bool AvMuxerAndroid::recording() const noexcept {
    return false; // TODO
}


bool AvMuxerAndroid::ready() const noexcept {
    return true; // TODO
}


}
