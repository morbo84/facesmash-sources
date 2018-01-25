#include "camera_android.h"
#include "face_bus_service.h"
#include "common/constants.h"
#include "locator/locator.hpp"
#include <SDL_surface.h>
#include <algorithm>
#include <atomic>
#include <cassert>
#include <tuple>


namespace gamee {


#ifdef __ANDROID__
extern std::atomic_bool cameraAndroidReady;
std::tuple<int, int> bindingGetCameraParams();
void bindingStartCamera();
void bindingStopCamera();
#else
static std::atomic_bool cameraAndroidReady{false};
std::tuple<int, int> bindingGetCameraParams() { return {}; }
void bindingStartCamera() {}
void bindingStopCamera() {}
#endif


void CameraAndroid::init() {
    std::lock_guard lck{frameMtx_};
    auto params = bindingGetCameraParams();
    width_ = std::get<0>(params);
    height_ = std::get<1>(params);
    auto bitsPerPixel = SDL_BITSPERPIXEL(internalFormat);
    assert(width_ >= 0 && height_ >= 0 && bitsPerPixel >= 0);
    auto pitch = (static_cast<size_t>(bitsPerPixel) * static_cast<size_t>(width_)) / 8;
    assert(pitch * 8U == static_cast<size_t>(bitsPerPixel * width_));
    size_ = pitch * height_;
    frame0_ = std::make_unique<char[]>(size_);
    frame1_ = std::make_unique<char[]>(size_);
    p0_ = frame0_.get();
    p1_ = frame1_.get();
    cameraAndroidReady = true;
}


void CameraAndroid::start() {
    bindingStartCamera();
}


void CameraAndroid::stop() {
    bindingStopCamera();
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
    SDL_ConvertPixels(width_, height_, cameraFormat, buf, width_ * SDL_BITSPERPIXEL(cameraFormat) / 8,
                      internalFormat, p0_, width_ * SDL_BITSPERPIXEL(internalFormat) / 8);
    swapPtrs();
    Locator::FaceBus::ref().enqueue(FrameAvailableEvent{});
}


void CameraAndroid::frame(std::function<void(const void *, int)> func) const noexcept {
    std::unique_lock lck{frameMtx_};
    func(p1_, size_);
}


void CameraAndroid::swapPtrs() {
    std::lock_guard lck{frameMtx_};
    std::swap(p0_, p1_);
}


}
