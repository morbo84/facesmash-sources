#include "audio_null.h"


namespace gamee {


void AudioNull::init() {}
void AudioNull::release() {}


bool AudioNull::isMute() const noexcept {
    return true;
}


}
