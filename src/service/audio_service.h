#ifndef FACE_SMASH_SERVICE_AUDIO_SERVICE_H
#define FACE_SMASH_SERVICE_AUDIO_SERVICE_H


#include "audio_service.h"


namespace gamee {


struct AudioService {
    bool mute() const noexcept;
    void mute(bool) noexcept;
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_SERVICE_H
