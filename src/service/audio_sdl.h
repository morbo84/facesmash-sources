#ifndef FACE_SMASH_SERVICE_AUDIO_SDL_H
#define FACE_SMASH_SERVICE_AUDIO_SDL_H


#include "audio_service.h"


namespace gamee {


struct AudioSDL: AudioService {
    bool mute() const noexcept override;
    void mute(bool) noexcept override;

    void pause() override;
    void resume() override;
    void halt() override;
    void rewind() override;

    void play(const AudioMusicResource &) override;
    void play(const AudioChunkResource &) override;

    void fadeIn(const AudioMusicResource &, int) override;
    int fadeIn(const AudioChunkResource &, int) override;

    void fadeOut(int) override;
    void fadeOut(int, int) override;

    bool playing() const noexcept override;
    bool playing(int) const noexcept override;

    bool paused() const noexcept override;
    bool paused(int) const noexcept override;

    bool fading() const noexcept override;
    bool fading(int) const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_SDL_H
