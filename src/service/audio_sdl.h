#ifndef FACE_SMASH_SERVICE_AUDIO_SDL_H
#define FACE_SMASH_SERVICE_AUDIO_SDL_H


#include "audio_service.h"


namespace gamee {


struct AudioSDL: AudioService {
    void init() override;
    void release() override;

    // TODO
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_SDL_H
