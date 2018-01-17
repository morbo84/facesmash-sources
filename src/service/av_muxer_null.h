#ifndef FACE_SMASH_SERVICE_MUXER_NULL_H
#define FACE_SMASH_SERVICE_MUXER_NULL_H


#include "av_muxer_service.h"


namespace gamee {


struct AvMuxerNull : AvMuxerService {
    void start(int width, int height) override;
    void frame(void*) override;
    void stop() override;
    bool isRecording() const noexcept override;
    std::string filePath() const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_MUXER_NULL_H
