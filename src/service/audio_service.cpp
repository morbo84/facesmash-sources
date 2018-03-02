#include <SDL_mixer.h>
#include "audio_service.h"


namespace gamee {


bool AudioService::mute() const noexcept {
    return (0 == Mix_Volume(-1, -1));
}


void AudioService::mute(bool set) noexcept {
    Mix_Volume(-1, set ? 0 : MIX_MAX_VOLUME);
}


}
