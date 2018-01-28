#ifndef FACE_SMASH_SERVICE_AUDIO_NULL_H
#define FACE_SMASH_SERVICE_AUDIO_NULL_H


#include "audio_service.h"


namespace gamee {


struct AudioNull: AudioService {
    void init() override;
    void release() override;

    bool isMute() const noexcept override;

    // TODO
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_NULL_H
