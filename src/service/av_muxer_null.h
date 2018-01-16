#ifndef FACE_SMASH_SERVICE_MUXER_NULL_H
#define FACE_SMASH_SERVICE_MUXER_NULL_H


#include "av_muxer_service.h"


namespace gamee {


struct AvMuxerNull : AvMuxerService {
    bool isRecording() const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_MUXER_NULL_H
