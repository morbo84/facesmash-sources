#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <tuple>
#include <SDL_surface.h>
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../factory/game_factory.h"
#include "../locator/locator.hpp"
#include "face_bus_service.h"
#include "camera_desktop.h"


namespace gamee {


static std::unique_ptr<char[]> makeFrame(size_t sz, char value = static_cast<char>(0xFF)) noexcept {
    auto ret = std::make_unique<char[]>(sz);
    std::fill_n(ret.get(), sz, value);
    return ret;
}


void CameraDesktop::start() {
    [[maybe_unused]] std::lock_guard lck{mtx_};
    stopped_ = false;
    cv_.notify_one();
}


void CameraDesktop::stop() {
    [[maybe_unused]] std::lock_guard lck{mtx_};
    stopped_ = true;
}


int CameraDesktop::width() const noexcept {
    return width_;
}


int CameraDesktop::height() const noexcept {
    return height_;
}


CameraDesktop::CameraDesktop() noexcept
	: camera_{0}
	, t_{&CameraDesktop::run, this}
{
    width_ = std::lround(camera_.get(cv::CAP_PROP_FRAME_WIDTH));
    height_ = camera_.get(cv::CAP_PROP_FRAME_HEIGHT);
    auto bitsPerPixel = SDL_BITSPERPIXEL(internalFormat);
    assert(width_ >= 0 && height_ >= 0 && bitsPerPixel >= 0);
    auto pitch = (static_cast<size_t>(bitsPerPixel) * static_cast<size_t>(width_)) / 8U;
    assert(pitch * 8U == static_cast<size_t>(bitsPerPixel * width_));
    size_ = pitch * height_;
    frame0_ = makeFrame(size_);
    frame1_ = makeFrame(size_);
    p0_ = frame0_.get();
    p1_ = frame1_.get();
    Locator::Dispatcher::ref().enqueue<CameraInitEvent>();
	Locator::Dispatcher::ref().enqueue<PermissionEvent>(PermissionType::CAMERA, PermissionStatus::GRANTED);
}


CameraDesktop::~CameraDesktop() noexcept {
    dying_ = true;
    stopped_ = false; // trust me :)
    cv_.notify_one();
    t_.join();
}


void CameraDesktop::setPixels() noexcept {
    convertToInternalFormat(img_, reinterpret_cast<std::byte*>(p0_));
    swapPtrs();
    Locator::FaceBus::ref().enqueue(FrameAvailableEvent{});
}


void CameraDesktop::run() noexcept {
    while(!dying_) {
        [[maybe_unused]] std::unique_lock lck{mtx_};
        if(stopped_)
            cv_.wait(lck, [this] { return !stopped_; });

        if(camera_.read(img_)) {
            setPixels();
        }
    }
}


void CameraDesktop::frame(std::function<void(const void *, int)> func) const noexcept {
    std::unique_lock lck{frameMtx_};
    func(p1_, size_);
}


void CameraDesktop::swapPtrs() noexcept {
    std::lock_guard lck{frameMtx_};
    std::swap(p0_, p1_);
}


void CameraDesktop::convertToInternalFormat(const cv::Mat& img, std::byte* argb) noexcept {
    const auto sdlSrcFmt = SDL_PIXELFORMAT_BGR24;
    SDL_ConvertPixels(img.cols, img.rows, sdlSrcFmt, img.ptr(), img.cols * SDL_BYTESPERPIXEL(sdlSrcFmt),
                      internalFormat, argb, img.cols * SDL_BYTESPERPIXEL(internalFormat));
}



}
