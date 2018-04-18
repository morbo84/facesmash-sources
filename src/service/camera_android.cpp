#include <tuple>
#include <atomic>
#include <cassert>
#include <algorithm>
#include <SDL_surface.h>
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../factory/game_factory.h"
#include "../locator/locator.hpp"
#include "face_bus_service.h"
#include "camera_android.h"


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


void CameraAndroid::init() noexcept {
    if(cameraAndroidReady) return;
    std::lock_guard lck{frameMtx_};
    auto params = bindingGetCameraParams();
    width_ = std::get<0>(params);
    height_ = std::get<1>(params);
    auto bitsPerPixel = SDL_BITSPERPIXEL(internalFormat);
    assert(width_ >= 0 && height_ >= 0 && bitsPerPixel >= 0);
    auto pitch = (static_cast<size_t>(bitsPerPixel) * static_cast<size_t>(width_)) / 8U;
    assert(pitch * 8U == static_cast<size_t>(bitsPerPixel * width_));
    size_ = pitch * height_;
    frame0_ = std::make_unique<char[]>(size_);
    frame1_ = std::make_unique<char[]>(size_);
    p0_ = frame0_.get();
    p1_ = frame1_.get();
    cameraAndroidReady = true;
    Locator::Dispatcher::ref().enqueue<CameraInitEvent>();
}


void CameraAndroid::start() {
    init();
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
    if(Locator::Permissions::ref().status(PermissionType::CAMERA) != PermissionStatus::GRANTED)
        Locator::Dispatcher::ref().connect<PermissionEvent>(this);
    else
        init();
}


CameraAndroid::~CameraAndroid() {
    cameraAndroidReady = false;
}


void CameraAndroid::setPixels(const void *buf) noexcept {
    SDL_ConvertPixels(width_, height_, cameraFormat, buf, width_ * SDL_BYTESPERPIXEL(cameraFormat),
                      internalFormat, p0_, width_ * SDL_BYTESPERPIXEL(internalFormat));
    swapPtrs();
    Locator::FaceBus::ref().enqueue(FrameAvailableEvent{});
}


void CameraAndroid::receive(const PermissionEvent& e) noexcept {
    if(e.permission == PermissionType::CAMERA && e.result == PermissionStatus::GRANTED) {
        Locator::Dispatcher::ref().disconnect<PermissionEvent>(this);
        init();
    }
}


void CameraAndroid::frame(std::function<void(const void *, int)> func) const noexcept {
    std::unique_lock lck{frameMtx_};
    func(p1_, size_);
}


void CameraAndroid::swapPtrs() noexcept {
    std::lock_guard lck{frameMtx_};
    std::swap(p0_, p1_);
}


}
