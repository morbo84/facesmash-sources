#include <algorithm>
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
static const char* visageLicense = "visage/504-932-294-611-606-835-011-303-246-311-003.vlc";


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
    , frame_{}
    , mtx_{}
    , cv_{}
    , dirty_{false}
    , end_{false}
    , t_{nullptr}
    , copy_{nullptr}
    , dst_{nullptr}
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

    lck.unlock();
    cv_.notify_one();

    SDL_WaitThread(t_, nullptr);

    SDL_FreeSurface(copy_);
    SDL_FreeSurface(dst_);

    if(image_) {
        vsReleaseImage(&image_);
    }
}


void EmoDetector::receive(const FrameAvailableEvent &) noexcept {
    std::unique_lock lck{mtx_, std::try_to_lock_t{}};

    if (lck && frame_ && !dirty_ && SDL_TICKS_PASSED(SDL_GetTicks(), timeout)) {
        Locator::Camera::ref().frame([this](const void* internal, int size) {
            auto frame = static_cast<const unsigned char *>(internal);
            std::copy_n(frame, size, frame_.get());
        });

        timeout = SDL_GetTicks() + interval;
        dirty_ = true;

        lck.unlock();
        cv_.notify_one();
    }
}


void EmoDetector::receive(const CameraInitEvent &) noexcept {
    const auto &camera = Locator::Camera::ref();
    const auto width = camera.width();
    const auto height = camera.height();

    frame_ = std::make_unique<unsigned char[]>((width * height * SDL_BITSPERPIXEL(internalFormat)) / 8);
    copy_ = SDL_CreateRGBSurfaceWithFormat(0, width, height, SDL_BITSPERPIXEL(detectorFormat), detectorFormat);

    const float mod = height > width ? 1.f * detectorHeight / height : 1.f * detectorHeight / width;
    const int w = width * mod;
    const int h = height * mod;

    dst_ = SDL_CreateRGBSurfaceWithFormat(0, w, h, SDL_BITSPERPIXEL(detectorFormat), detectorFormat);
    image_ = vsCreateImage(h > w ? vsSize(w, h) : vsSize(h, w), VS_DEPTH_8U, visageChannels);

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
            SDL_ConvertPixels(detector.copy_->w, detector.copy_->h,
                              internalFormat, detector.frame_.get(), detector.copy_->w * SDL_BITSPERPIXEL(internalFormat) / 8,
                              detectorFormat, detector.copy_->pixels, detector.copy_->pitch);

            SDL_BlitScaled(detector.copy_, nullptr, detector.dst_, nullptr);

            if(detector.dst_->h > detector.dst_->w) {
                const auto *pixels = static_cast<const unsigned char*>(detector.dst_->pixels);
                std::copy_n(pixels, detector.dst_->pitch * detector.dst_->h, detector.image_->imageData);
            } else {
                const auto height = detector.dst_->h;
                const auto width = detector.dst_->w;
                const auto pitch = height * visageChannels;
                const auto nbytes = detector.dst_->pitch / detector.dst_->w;
                const auto *pixels = static_cast<const unsigned char*>(detector.dst_->pixels);

                for(auto row = 0; row < height; ++row) {
                    for(auto col = 0; col < width; ++col) {
                        const auto *from = pixels + row * detector.dst_->pitch + col * nbytes;
                        auto *to = detector.image_->imageData + pitch * (width - col - 1) + row * visageChannels;
                        std::copy_n(from, visageChannels, to);
                    }
                }
            }

            auto* statuses = detector.tracker_.track(detector.image_->width, detector.image_->height, detector.image_->imageData,
                                                     &faceData, VISAGE_FRAMEGRABBER_FMT_RGB, VISAGE_FRAMEGRABBER_ORIGIN_TL, 0, -1, 1);

            if(statuses[0] == TRACK_STAT_OK) {
                std::array<float, 7> probs;

                if(detector.analyzer_.estimateEmotion(detector.image_, &faceData, probs.data())) {
                    auto [face, prob] = estimateEmotion(probs);
                    Locator::FaceBus::ref().enqueue({ face, prob });
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
