#include <algorithm>
#include <cstddef>
#include <string>
#include <SDL_rect.h>
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
static const char* visageLicense = "visage/478-415-978-378-371-879-775-031-990-195-624.vlc";


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
    : timeout{SDL_GetTicks()}
    , tracker_{visageTrackingCfg().c_str()}
    , analyzer_{}
    , image_{nullptr}
    , mtx_{}
    , cv_{}
    , dirty_{false}
    , end_{false}
    , t_{nullptr}
    , copy_{nullptr}
    , dst_{nullptr}
{
    analyzer_.init(visageDataPath().c_str());
    Locator::Dispatcher::ref().sink<CameraInitEvent>().connect<&EmoDetector::onCameraInit>(this);
    Locator::Dispatcher::ref().sink<FrameAvailableEvent>().connect<&EmoDetector::onFrameAvailable>(this);
}


EmoDetector::~EmoDetector() {
    std::unique_lock lck{mtx_};

    Locator::Dispatcher::ref().sink<FrameAvailableEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<CameraInitEvent>().disconnect(this);

    end_ = true;

    lck.unlock();
    cv_.notify_one();

    SDL_WaitThread(t_, nullptr);

    SDL_FreeSurface(copy_);
    SDL_FreeSurface(dst_);

    if(image_) {
        vsReleaseImage(&image_);
    }
}


void EmoDetector::onFrameAvailable(const FrameAvailableEvent &) noexcept {
    std::unique_lock lck{mtx_, std::try_to_lock_t{}};

    if (lck && !dirty_ && SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
        Locator::Camera::ref().frame([this](const void* internal, int size) {
            auto frame = static_cast<const std::byte*>(internal);
            std::copy_n(frame, size, static_cast<std::byte*>(copy_->pixels));
        });

        timeout = SDL_GetTicks() + interval;
        dirty_ = true;

        lck.unlock();
        cv_.notify_one();
    }
}


void EmoDetector::onCameraInit(const CameraInitEvent &) noexcept {
    const auto &camera = Locator::Camera::ref();
    const auto width = camera.width();
    const auto height = camera.height();

    copy_ = SDL_CreateRGBSurfaceWithFormat(0, width, height, SDL_BITSPERPIXEL(detectorFormat), detectorFormat);
    dst_ = SDL_CreateRGBSurfaceWithFormat(0, detectorWidth, detectorHeight, SDL_BITSPERPIXEL(detectorFormat), detectorFormat);
    image_ = vsCreateImage(vsSize(detectorWidth, detectorHeight), VS_DEPTH_8U, visageChannels);

    const auto rc = (1. * width) / height;
    const auto rl = (1. * logicalWidth) / logicalHeight;
    const auto w = rc >= rl ? (1 * height * logicalWidth) / logicalHeight : (1. * camera.width());
    const auto h = rc >= rl ? (1. * camera.height()) : (1 * width * logicalHeight) / logicalWidth;
    const auto x = rc >= rl ? (width - w) * 0.5 : 0.;
    const auto y = rc >= rl ? 0. : (height - h) * 0.5;
    rect_ = SDL_Rect{static_cast<int>(std::lround(x)),
            static_cast<int>(std::lround(y)),
            static_cast<int>(std::lround(w)),
            static_cast<int>(std::lround(h))};

    t_ = SDL_CreateThread(&EmoDetector::analyzeCurrentFrame, "detector", this);
}


int EmoDetector::analyzeCurrentFrame(void *ptr) {
    auto &detector = *static_cast<EmoDetector *>(ptr);
    VisageSDK::FaceData faceData;

    std::unique_lock lck{detector.mtx_};

    while(!detector.end_) {
        if(!detector.dirty_) {
            detector.cv_.wait(lck, [&detector] { return detector.dirty_.load() || detector.end_; });
        }

        if(!detector.end_) {
            SDL_BlitScaled(detector.copy_, &detector.rect_, detector.dst_, nullptr);
            const auto *pixels = static_cast<const unsigned char*>(detector.dst_->pixels);
            std::copy_n(pixels, detector.dst_->pitch * detector.dst_->h, detector.image_->imageData);

            auto* statuses = detector.tracker_.track(detector.image_->width, detector.image_->height, detector.image_->imageData,
                                                     &faceData, VISAGE_FRAMEGRABBER_FMT_RGBA, VISAGE_FRAMEGRABBER_ORIGIN_TL, 0, -1, 1);

            if(statuses[0] == TRACK_STAT_OK) {
                std::array<float, 7> probs;

                if(detector.analyzer_.estimateEmotion(detector.image_, &faceData, probs.data())) {
                    auto [face, prob] = estimateEmotion(probs);

                    // future improvements: find a better way, we cannot copy the scaled frame each and every time
                    std::unique_ptr<unsigned char[]> data = std::make_unique<unsigned char[]>(
                            (detectorWidth * detectorHeight * SDL_BITSPERPIXEL(detectorFormat)) / 8);
                    std::copy_n(pixels, detector.dst_->pitch * detector.dst_->h, data.get());

                    Locator::FaceBus::ref().enqueue({ face, prob, std::move(data) });
                }
            }
        }

        detector.dirty_ = false;
    }

    return 0;
}


std::pair<FaceType, float> EmoDetector::estimateEmotion(const std::array<float, length> &prob) {
    const auto it = std::max_element(prob.cbegin(), prob.cend());
    const auto diff = std::distance(prob.cbegin(), it);
    const auto emo = static_cast<Emotion>(diff);

    switch (emo) {
    case Emotion::anger:
        return { FaceType::ANGRY, *it };
    case Emotion::disgust:
        return { FaceType::DISGUSTED, *it };
    case Emotion::fear:
        return { FaceType::FEARFUL, *it };
    case Emotion::happiness:
        return { FaceType::HAPPY, *it };
    case Emotion::sadness:
        return { FaceType::SAD, *it };
    case Emotion::surprise:
        return { FaceType::SURPRISED, *it };
    default: // suppress warnings due to neutral face
        break;
    }

    return { FaceType::ANGRY, 0.f };
}


} // namespace gamee
