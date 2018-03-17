#ifndef FACE_SMASH_SERVICE_RECORDER_ANDROID_H
#define FACE_SMASH_SERVICE_RECORDER_ANDROID_H


#include <atomic>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <condition_variable>
#include "av_recorder_service.h"


namespace gamee {


struct AvRecorderAndroid : AvRecorderService {
    using VideoFrame = std::pair<const unsigned char*, delta_type>;

    ~AvRecorderAndroid() override;

    void start(int, int) override;
    void frame(const unsigned char *, delta_type) override;
    void stop() override;
    bool recording() const noexcept override;
    bool ready() const noexcept override;

    bool supportExport() const override;
    void exportMedia() const override;

private:
    void recordVideo(int width, int height);

    std::thread t_;
    std::atomic_bool stopped_{true};
    std::condition_variable cv_;
    mutable std::mutex mtx_;
    VideoFrame frame_{nullptr, 0U};
    std::atomic_bool ready_{true};
};


}


#endif // FACE_SMASH_SERVICE_RECORDER_ANDROID_H
