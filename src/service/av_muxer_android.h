#ifndef FACE_SMASH_SERVICE_MUXER_ANDROID_H
#define FACE_SMASH_SERVICE_MUXER_ANDROID_H


#include "av_muxer_service.h"


namespace gamee {


struct AvMuxerAndroid : AvMuxerService {
    bool isRecording() const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_MUXER_ANDROID_H
