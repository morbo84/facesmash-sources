#ifndef FACE_SMASH_SERVICE_AUDIO_SDL_H
#define FACE_SMASH_SERVICE_AUDIO_SDL_H


#include "audio_service.h"


namespace gamee {


struct AudioSdl: AudioService {
    bool isMute() const noexcept override;

    // TODO
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_SDL_H
