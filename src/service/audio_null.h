#ifndef FACE_SMASH_SERVICE_AUDIO_NULL_H
#define FACE_SMASH_SERVICE_AUDIO_NULL_H


#include "audio_service.h"


namespace gamee {


struct AudioNull: AudioService {
    bool mute() const noexcept override;
    void mute(bool) noexcept override;

    void pause() override;
    void resume() override;
    void halt() override;

    void play(const AudioMusicResource &) override;
    void play(const AudioChunkResource &) override;

    void fadeIn(const AudioMusicResource &, int) override;
    int fadeIn(const AudioChunkResource &, int) override;

    void fadeOut(int) override;
    void fadeOut(int, int) override;
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_NULL_H
