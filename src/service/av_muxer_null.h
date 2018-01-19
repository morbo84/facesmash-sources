#ifndef FACE_SMASH_SERVICE_MUXER_NULL_H
#define FACE_SMASH_SERVICE_MUXER_NULL_H


#include "av_muxer_service.h"


namespace gamee {


struct AvMuxerNull: AvMuxerService {
    void start(int, int) override;
    void frame(std::unique_ptr<unsigned char[]>, delta_type) override;
    void stop() override;
    bool recording() const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_MUXER_NULL_H
