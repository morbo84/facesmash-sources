#ifndef FACE_SMASH_SERVICE_CAMERA_DESKTOP_H
#define FACE_SMASH_SERVICE_CAMERA_DESKTOP_H


#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <memory>
#include <thread>
#include "../common/constants.h"
#include "camera_service.h"

#ifdef FACESMASH_DESKTOP
#include "opencv2/videoio.hpp"
#endif


namespace gamee {


struct CameraDesktop final : CameraService {
    CameraDesktop() noexcept;
    ~CameraDesktop() noexcept;

    void start() override;
    void stop() override;

    int width() const noexcept override;
    int height() const noexcept override;
    void frame(std::function<void(const void *, int)>) const noexcept override;

private:
#ifdef FACESMASH_DESKTOP
    void run() noexcept;
    void setPixels() noexcept;
    void swapPtrs() noexcept;
    void convertToInternalFormat(const cv::Mat& img, std::byte* argb) noexcept;

    cv::VideoCapture camera_;
    cv::Mat img_;
    std::unique_ptr<char[]> frame0_;
    std::unique_ptr<char[]> frame1_;
    char* p0_;
    char* p1_;
    std::thread t_;
    std::condition_variable cv_;
    std::mutex mtx_;
    mutable std::mutex frameMtx_;
    int width_{logicalWidth/2};
    int height_{logicalHeight/2};
    size_t size_{0};
    std::atomic_bool dying_{false};
    bool stopped_{true};
#endif
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_DESKTOP_H
