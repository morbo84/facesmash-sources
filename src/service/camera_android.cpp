#include "camera_android.h"
#include <algorithm>
#include <cassert>
#include <tuple>

namespace gamee {

std::tuple<int, int, int> facesmashGetCameraParams();


void CameraAndroid::init() {
    std::lock_guard<std::mutex> lck{frameMtx_};
    auto params = facesmashGetCameraParams();
    width_ = std::get<0>(params);
    height_ = std::get<1>(params);
    auto bitsPerPixel = std::get<2>(params);
    assert(width_ >= 0 && height_ >= 0 && bitsPerPixel >= 0);
    auto pitch = (static_cast<size_t>(bitsPerPixel) * static_cast<size_t>(width_)) / 8;
    assert(pitch * 8U == static_cast<size_t>(bitsPerPixel * width_));
    size_ = pitch * height_;
    frame_ = new char[size_];
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
    std::lock_guard<std::mutex> lck{frameMtx_};
    std::copy(cbuf, cbuf + size_, frame_);
}


void CameraAndroid::frame(std::function<void(const void *, int)> func) const noexcept {
    std::lock_guard<std::mutex> lck{frameMtx_};
    func(frame_, size_);
}


}
