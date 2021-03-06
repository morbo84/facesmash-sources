#include <SDL_mixer.h>
#include "../resource/audio_resource.h"
#include "audio_sdl.h"


namespace gamee {


bool AudioSDL::mute() const noexcept {
    return (0 == Mix_VolumeMusic(-1));
}


void AudioSDL::mute(bool set) noexcept {
    const auto volume = set ? 0 : MIX_MAX_VOLUME;
    Mix_VolumeMusic(volume);
    Mix_Volume(-1, volume);
}


void AudioSDL::pause() {
    Mix_PauseMusic();
    Mix_Pause(-1);
}


void AudioSDL::resume() {
    Mix_ResumeMusic();
    Mix_Resume(-1);
}


void AudioSDL::halt() {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
}


void AudioSDL::rewind() {
    Mix_RewindMusic();
}


void AudioSDL::play(const AudioMusicResource &music) {
    Mix_PlayMusic(music, -1);
}


void AudioSDL::play(const AudioChunkResource &chunk) {
    Mix_PlayChannel(-1, chunk, 0);
}


void AudioSDL::fadeIn(const AudioMusicResource &music, int ms) {
    Mix_FadeInMusic(music, -1, ms);
}


int AudioSDL::fadeIn(const AudioChunkResource &chunk, int ms) {
    return Mix_FadeInChannel(-1, chunk, 0, ms);
}


void AudioSDL::fadeOut(int ms) {
    Mix_FadeOutMusic(ms);
}


void AudioSDL::fadeOut(int channel, int ms) {
    Mix_FadeOutChannel(channel, ms);
}


bool AudioSDL::playing() const noexcept {
    return (1 == Mix_PlayingMusic());
}


bool AudioSDL::playing(int channel) const noexcept {
    return (1 == Mix_Playing(channel));
}


bool AudioSDL::paused() const noexcept {
    return (1 == Mix_PausedMusic());
}


bool AudioSDL::paused(int channel) const noexcept {
    return (1 == Mix_Paused(channel));
}


bool AudioSDL::fading() const noexcept {
    return !(Mix_FadingMusic() == MIX_NO_FADING);
}


bool AudioSDL::fading(int channel) const noexcept {
    return !(Mix_FadingChannel(channel) == MIX_NO_FADING);
}


}
