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


struct SDL_Thread;


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
    void exportMedia() override;

private:
    static int recordVideo(void *);

#ifdef __ANDROID__
    void waitRecording();

    SDL_Thread *t_{nullptr};
    int width{0};
    int height{0};
    std::atomic_bool stopped_{true};
    std::condition_variable cv_{};
    mutable std::mutex mtx_{};
    VideoFrame frame_{nullptr, 0U};
    std::atomic_bool ready_{true};
#endif
};


}


#endif // FACE_SMASH_SERVICE_RECORDER_ANDROID_H
