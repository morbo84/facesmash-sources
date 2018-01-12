#include "camera_android.h"
#include "face_bus_service.h"
#include "../locator/locator.hpp"
#include <algorithm>
#include <atomic>
#include <cassert>
#include <tuple>


namespace gamee {


#ifdef __ANDROID__
extern std::atomic_bool cameraAndroidReady;
std::tuple<int, int, int> facesmashGetCameraParams();
#else
static std::atomic_bool cameraAndroidReady{false};
std::tuple<int, int, int> facesmashGetCameraParams() { return {}; }
#endif


void CameraAndroid::init() {
    std::lock_guard lck{frameMtx_};
    auto params = facesmashGetCameraParams();
    width_ = std::get<0>(params);
    height_ = std::get<1>(params);
    auto bitsPerPixel = std::get<2>(params);
    assert(width_ >= 0 && height_ >= 0 && bitsPerPixel >= 0);
    auto pitch = (static_cast<size_t>(bitsPerPixel) * static_cast<size_t>(width_)) / 8;
    assert(pitch * 8U == static_cast<size_t>(bitsPerPixel * width_));
    size_ = pitch * height_;
    frame_ = std::make_unique<char[]>(size_);
    cameraAndroidReady = true;
}


void CameraAndroid::start() {
    // TODO
}


void CameraAndroid::stop() {
    // TODO
}


int CameraAndroid::width() const noexcept {
    return width_;
}


int CameraAndroid::height() const noexcept {
    return height_;
}


CameraAndroid::CameraAndroid() {
    init();
}


void CameraAndroid::setPixels(const void *buf) {
    auto cbuf = static_cast<const char *>(buf);
    std::unique_lock lck{frameMtx_};
    std::copy(cbuf, cbuf + size_, frame_.get());
    lck.unlock();
    Locator::FaceBus::ref().enqueue(FrameAvailableEvent{});
}


void CameraAndroid::frame(std::function<void(const void *, int)> func) const noexcept {
    std::lock_guard lck{frameMtx_};
    func(frame_.get(), size_);
}


}
