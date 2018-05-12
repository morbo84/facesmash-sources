#include <algorithm>
#include <array>
#include <string>
#include <SDL_surface.h>
#include <SDL_system.h>
#include <SDL_thread.h>
#include "../common/constants.h"
#include "../locator/locator.hpp"
#include "emo_detector.h"


static const std::string& visageTrackingCfg() {
#ifdef __ANDROID__
    static const auto ret = std::string{SDL_AndroidGetInternalStoragePath()} + "/visage/Facial Features Tracker - High.cfg";
#else
    static const std::string ret = "visage/Facial Features Tracker - High.cfg";
#endif
    return ret;
}


static const std::string& visageDataPath() {
#ifdef __ANDROID__
    static const auto ret = std::string{SDL_AndroidGetInternalStoragePath()} + "/visage/bdtsdata/LBF/vfadata";
#else
    static const std::string ret = "visage/bdtsdata/LBF/vfadata";
#endif
    return ret;
}


#ifndef __ANDROID__
static const char* visageLicense = "visage/813-721-696-114-345-714-410-682-472-210-486.vlc";


// neccessary prototype declaration for licensing
namespace VisageSDK {
    void initializeLicenseManager(const char *licenseKeyFileName);
}

struct DummyVisageInitializer {
    DummyVisageInitializer() noexcept {
        //initialize licensing
        VisageSDK::initializeLicenseManager(visageLicense);
    }
};

static DummyVisageInitializer dummy_;
#endif


namespace gamee {


EmoDetector::EmoDetector()
    : width_{-1}
    , height_{-1}
    , tracker_{visageTrackingCfg().c_str()}
    , image_{nullptr}
    , internalSize_{0U}
    , internal_{}
    , dirty_{false}
    , end_{false}
    , t_{nullptr}
{
    analyzer_.init(visageDataPath().c_str());
    Locator::Dispatcher::ref().connect<CameraInitEvent>(this);
    Locator::Dispatcher::ref().connect<FrameAvailableEvent>(this);
}


EmoDetector::~EmoDetector() {
    std::unique_lock lck{mtx_};
    Locator::Dispatcher::ref().disconnect<FrameAvailableEvent>(this);
    Locator::Dispatcher::ref().disconnect<CameraInitEvent>(this);
    end_ = true;
    dirty_ = true;
    lck.unlock();
    cv_.notify_one();

    SDL_WaitThread(t_, nullptr);
    t_ = nullptr;

    if(image_) {
        vsReleaseImage(&image_);
    }
}


void EmoDetector::start(int width, int height) {
    width_ = width;
    height_ = height;
    image_ = vsCreateImage(width > height ? vsSize(height, width) : vsSize(width, height), VS_DEPTH_8U, 3);
    internalSize_ = (static_cast<size_t>(width * height) * SDL_BITSPERPIXEL(internalFormat)) / 8;
    internal_ = std::make_unique<unsigned char[]>(internalSize_);
    t_ = SDL_CreateThread(&EmoDetector::analyzeCurrentFrame, "detector", this);
}


void EmoDetector::receive(const FrameAvailableEvent &) noexcept {
    if (dirty_) return;
    Locator::Camera::ref().frame([this](const void* internal, int) {
        auto frame = static_cast<const unsigned char*>(internal);
        std::copy_n(frame, internalSize_, internal_.get());
    });
    dirty_ = true;
    cv_.notify_one();
}


void EmoDetector::receive(const CameraInitEvent&) noexcept {
    const auto& camera = Locator::Camera::ref();
    start(camera.width(), camera.height());
}


int EmoDetector::analyzeCurrentFrame(void *ptr) {
    auto &detector = *static_cast<EmoDetector *>(ptr);
    VisageSDK::FaceData faceData;

    while(true) {
        std::unique_lock lck{detector.mtx_};
        if(!detector.dirty_)
            detector.cv_.wait(lck, [&detector] { return bool{detector.dirty_}; });

        if(detector.end_) break;

        // TODO: height_ >= width_ is constant...
        if (detector.height_ >= detector.width_) {
            ARGBtoRGB(detector.internal_.get(), *detector.image_, detector.width_, detector.height_);
        }
        else {
            ARGBtoRGBRotated(detector.internal_.get(), *detector.image_, detector.width_, detector.height_);
        }

        auto* statuses = detector.tracker_.track(detector.image_->width, detector.image_->height,
                                                 detector.image_->imageData, &faceData,
                                                 VISAGE_FRAMEGRABBER_FMT_RGB, VISAGE_FRAMEGRABBER_ORIGIN_TL,
                                                 0, -1, 1);

        if(statuses[0] == TRACK_STAT_OK) {
            std::array<float, 7> prob;
            if(detector.analyzer_.estimateEmotion(detector.image_, &faceData, prob.data())) {
                if(auto emo = estimateEmotion(prob.data())) {
                    Locator::FaceBus::ref().enqueue({*emo});
                }
            }
        }

        detector.dirty_ = false;
    }

    return 0;
}


std::optional<FaceType> EmoDetector::estimateEmotion(float* prob) {
    constexpr auto minProb = 0.80f;
    auto found = std::max_element(prob, prob + 7);
    if(found == prob + 7 || *found < minProb) return {};
    auto diff = std::distance(prob, found);
    auto emo = static_cast<Emotion>(diff);
    switch (emo) {
        case Emotion::anger:
            return FaceType::ANGRY;
        case Emotion::disgust:
            return FaceType::DISGUSTED;
        case Emotion::fear:
            return FaceType::FEARFUL;
        case Emotion::happiness:
            return FaceType::HAPPY;
        case Emotion::sadness:
            return FaceType::SAD;
        case Emotion::surprise:
            return FaceType::SURPRISED;
        default:
            break;
    }

    return {};
}


void EmoDetector::ARGBtoRGB(const unsigned char* argb, VsImage& buff, int width, int height) {
    constexpr auto dstFormat = SDL_PIXELFORMAT_RGB888;
    SDL_ConvertPixels(width, height, internalFormat, argb, width * SDL_BITSPERPIXEL(internalFormat),
                      dstFormat, buff.imageData, width * SDL_BITSPERPIXEL(dstFormat));
}


void EmoDetector::ARGBtoRGBRotated(const unsigned char* argb, VsImage& buff, int width, int height) {
    // TODO: this shit sucks...
    for(auto i = 0; i < height; ++i) {
        for(auto j = 0; j < width; ++j) {
            auto* src = argb + 4 * ((i * width) + j) + 1; // +1 is to jump the A channel
            auto* dst = buff.imageData + 3 * (((width - j - 1) * height) + i);
            std::copy_n(src, 3, dst);
        }
    }
}


} // namespace gamee
