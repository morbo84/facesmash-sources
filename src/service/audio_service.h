#ifndef FACE_SMASH_SERVICE_AUDIO_SERVICE_H
#define FACE_SMASH_SERVICE_AUDIO_SERVICE_H


#include "audio_service.h"


namespace gamee {


struct AudioMusicResource;
struct AudioChunkResource;


struct AudioService {
    virtual ~AudioService() = default;

    virtual bool mute() const noexcept = 0;
    virtual void mute(bool) noexcept = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void halt() = 0;
    virtual void rewind() = 0;

    virtual void play(const AudioMusicResource &) = 0;
    virtual void play(const AudioChunkResource &) = 0;

    virtual void fadeIn(const AudioMusicResource &, int) = 0;
    virtual int fadeIn(const AudioChunkResource &, int) = 0;

    virtual void fadeOut(int) = 0;
    virtual void fadeOut(int, int) = 0;

    virtual bool playing() const noexcept = 0;
    virtual bool playing(int) const noexcept = 0;

    virtual bool paused() const noexcept = 0;
    virtual bool paused(int) const noexcept = 0;

    virtual bool fading() const noexcept = 0;
    virtual bool fading(int) const noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_SERVICE_H
