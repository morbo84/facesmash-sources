#ifndef FACE_SMASH_SERVICE_MUXER_ANDROID_H
#define FACE_SMASH_SERVICE_MUXER_ANDROID_H


#include <mutex>
#include <thread>
#include <condition_variable>
#include "av_muxer_service.h"


namespace gamee {


struct AvMuxerAndroid: AvMuxerService {
    void start(int, int) override;
    void frame(const unsigned char *, delta_type) override;
    void stop() override;
    bool recording() const noexcept override;
    bool ready() const noexcept override;

private:
    std::thread t_;
    std::condition_variable cv_;
    mutable std::mutex mtx_;
    bool run{false};
};


}


#endif // FACE_SMASH_SERVICE_MUXER_ANDROID_H
