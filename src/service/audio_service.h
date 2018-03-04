#ifndef FACE_SMASH_SERVICE_AUDIO_SERVICE_H
#define FACE_SMASH_SERVICE_AUDIO_SERVICE_H


#include "audio_service.h"


namespace gamee {


struct AudioMusicResource;
struct AudioChunkResource;


struct AudioService {
    bool mute() const noexcept;
    void mute(bool) noexcept;

    void pause();
    void resume();
    void halt();

    void play(const AudioMusicResource &);
    void play(const AudioChunkResource &);

    void fadeIn(const AudioMusicResource &, int);
    int fadeIn(const AudioChunkResource &, int);

    void fadeOut(int);
    void fadeOut(int, int);
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_SERVICE_H
