#ifndef FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H
#define FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H


#include <string>


namespace gamee {


struct AvMuxerService {
    virtual ~AvMuxerService() = default;

    virtual void start(int width, int height) = 0;
    virtual void frame(void*) = 0;
    virtual void stop() = 0;
    virtual bool isRecording() const noexcept = 0;
    virtual std::string filePath() const noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H
