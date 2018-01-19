#ifndef FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H
#define FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H


#include <memory>
#include "../common/types.h"
#include "../resource/frame_resource.h"


namespace gamee {


struct AvMuxerService {
    virtual ~AvMuxerService() = default;

    virtual void start(int, int) = 0;
    virtual void frame(std::unique_ptr<FrameTexture>, delta_type) = 0;
    virtual void stop() = 0;
    virtual bool recording() const noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H