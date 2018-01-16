#ifndef FACE_SMASH_SERVICE_MUXER_ANDROID_H
#define FACE_SMASH_SERVICE_MUXER_ANDROID_H


#include "av_muxer_service.h"
#include <thread>
#include <condition_variable>
#include <mutex>


namespace gamee {


struct AvMuxerAndroid : AvMuxerService {
    void start(int width, int height) override;
    void frame(void*) override;
    void stop() override;
    bool isRecording() const noexcept override;
    std::string filePath() const noexcept override;

private:
    std::thread t_;
    std::condition_variable cv_;
    mutable std::mutex mtx_;
};


}


#endif // FACE_SMASH_SERVICE_MUXER_ANDROID_H
