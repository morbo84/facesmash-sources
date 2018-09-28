#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <tuple>
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
std::tuple<int, int, int> bindingGetCameraParams();
void bindingStartCamera();
void bindingStopCamera();
#else
static std::atomic_bool cameraAndroidReady{false};
static std::tuple<int, int, int> bindingGetCameraParams() { return {}; }
static void bindingStartCamera() {}
static void bindingStopCamera() {}
#endif


static std::unique_ptr<char[]> makeFrame(size_t sz, char value = static_cast<char>(0xFF)) noexcept {
    auto ret = std::make_unique<char[]>(sz);
    std::fill_n(ret.get(), sz, value);
    return ret;
}


void CameraAndroid::init() noexcept {
    if(cameraAndroidReady) return;
    auto params = bindingGetCameraParams();
    std::lock_guard lck{frameMtx_};
    width_ = std::get<0>(params);
    height_ = std::get<1>(params);
    rotation_ = std::get<2>(params);
    auto bitsPerPixel = SDL_BITSPERPIXEL(internalFormat);
    assert(width_ >= 0 && height_ >= 0 && bitsPerPixel >= 0);
    auto pitch = (static_cast<size_t>(bitsPerPixel) * static_cast<size_t>(width_)) / 8U;
    assert(pitch * 8U == static_cast<size_t>(bitsPerPixel * width_));
    size_ = pitch * height_;
    frame0_ = makeFrame(size_);
    frame1_ = makeFrame(size_);
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
    return (rotation_ == 0 || rotation_ == 180) ? width_ : height_;
}


int CameraAndroid::height() const noexcept {
    return (rotation_ == 0 || rotation_ == 180) ? height_ : width_;
}


CameraAndroid::CameraAndroid() {
    if(Locator::Permissions::ref().status(PermissionType::CAMERA) != PermissionStatus::GRANTED)
        Locator::Dispatcher::ref().sink<PermissionEvent>().connect(this);
    else
        init();
}


CameraAndroid::~CameraAndroid() {
    cameraAndroidReady = false;
}


void CameraAndroid::setPixels(const void *buf) noexcept {
    yuvNV21ToARGB(buf, p0_);
    swapPtrs();
    Locator::FaceBus::ref().enqueue(FrameAvailableEvent{});
}


void CameraAndroid::receive(const PermissionEvent& e) noexcept {
    if(e.permission == PermissionType::CAMERA && e.result == PermissionStatus::GRANTED) {
        Locator::Dispatcher::ref().sink<PermissionEvent>().disconnect(this);
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


static char clamp(int x) {
    unsigned y;
    return static_cast<char>(!(y=x>>8) ? x : (0xff ^ (y>>24)));
}


static int pos0deg(int i, int j, int chNum, int width, int) noexcept {
    return chNum * (i * width + j);
}


static int pos90deg(int i, int j, int chNum, int width, int height) noexcept {
    return chNum * (j * height + width - 1 - i);
}


static int pos180deg(int i, int j, int chNum, int width, int height) noexcept {
    return chNum * (width * (height - 1 - i) + (width - j - 1));
}


static int pos270deg(int i, int j, int chNum, int width, int height) noexcept {
    return chNum * ((width - 1 - j) * height + height - 1 - i);
}


using RotFn = int(*)(int, int, int, int, int);


static std::array<RotFn, 4> rotationFunctions{pos0deg, pos90deg, pos180deg, pos270deg};


void CameraAndroid::yuvNV21ToARGB(const void* yuv, char* argb) noexcept {
    const int chNum = 4;
    const int frameSize = width_ * height_;
    const auto buf = static_cast<const char*>(yuv);
    const auto rotatedPos = rotationFunctions[rotation_ / 90];

    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            int y = 0xff & ((int) buf[i * width_ + j]);
            int v = 0xff & ((int) buf[frameSize + (i >> 1) * width_ + (j & ~1) + 0]);
            int u = 0xff & ((int) buf[frameSize + (i >> 1) * width_ + (j & ~1) + 1]);
            y = y < 16 ? 16 : y;

            int a0 = 1192 * (y - 16);
            int a1 = 1634 * (v - 128);
            int a2 = 832 * (v - 128);
            int a3 = 400 * (u - 128);
            int a4 = 2066 * (u - 128);

            int r = (a0 + a1) >> 10;
            int g = (a0 - a2 - a3) >> 10;
            int b = (a0 + a4) >> 10;

            auto pos = rotatedPos(i, j, chNum, width_, height_);
            argb[pos] = clamp(b);
            argb[pos + 1] = clamp(g);
            argb[pos +  2] = clamp(r);
            // argb[pos + 3] = 0xFF; // already initialized to 0xFF
        }
    }
}



}
