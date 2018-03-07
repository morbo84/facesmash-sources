#include <SDL_mixer.h>
#include "audio_null.h"


namespace gamee {


bool AudioNull::mute() const noexcept {
    return true;
}


void AudioNull::mute(bool) noexcept {}


void AudioNull::pause() {}


void AudioNull::resume() {}


void AudioNull::halt() {}


void AudioNull::rewind() {}


void AudioNull::play(const AudioMusicResource &) {}


void AudioNull::play(const AudioChunkResource &) {}


void AudioNull::fadeIn(const AudioMusicResource &, int) {}


int AudioNull::fadeIn(const AudioChunkResource &, int) {
    return {};
}


void AudioNull::fadeOut(int) {}


void AudioNull::fadeOut(int, int) {}


bool AudioNull::playing() const noexcept {
    return false;
}


bool AudioNull::playing(int) const noexcept {
    return false;
}


bool AudioNull::paused() const noexcept {
    return false;
}


bool AudioNull::paused(int) const noexcept {
    return false;
}


bool AudioNull::fading() const noexcept {
    return false;
}


bool AudioNull::fading(int) const noexcept {
    return false;
}


}
