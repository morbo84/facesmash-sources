#include "emo_detector.h"
#include "../locator/locator.hpp"
#include <algorithm>
#include <array>

#ifdef __ANDROID__
static const char* visageTrackingCfg = "/data/data/com.cynny.gamee.facesmash/files/visage/Facial Features Tracker - High.cfg";
static const char* visageDataPath = "/data/data/com.cynny.gamee.facesmash/files/visage/bdtsdata/LBF/vfadata";
#else
static const char* visageTrackingCfg = "visage/Facial Features Tracker - High.cfg";
static const char* visageDataPath = "visage/bdtsdata/LBF/vfadata";
static const char* visageLicense = "visage/578-496-411-691-522-273-235-359-916-935-253.vlc";

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


EmoDetector::EmoDetector(int width, int height)
    : width_{width}
    , height_{height}
    , tracker_{visageTrackingCfg}
    , image_{vsCreateImage(width > height ? vsSize(height, width) : vsSize(width, height), VS_DEPTH_8U, 3)}
    , dirty_{false}
    , end_{false}
    , t_{&EmoDetector::analyzeCurrentFrame, this}
{
    analyzer_.init(visageDataPath);
    Locator::Dispatcher::ref().connect<FrameAvailableEvent>(this);
}


EmoDetector::~EmoDetector() {
    std::unique_lock lck{mtx_};
    Locator::Dispatcher::ref().disconnect<FrameAvailableEvent>(this);
    end_ = true;
    dirty_ = true;
    lck.unlock();
    cv_.notify_one();
    t_.join();
}


void EmoDetector::receive(const FrameAvailableEvent &) noexcept {
    // TODO: height_ >= width_ is constant...
    if (dirty_) return;
    std::unique_lock lck{mtx_};
    Locator::Camera::ref().frame([this](const void *yuv, int) {
        if (height_ >= width_)
            yuvN21toRGB(static_cast<const unsigned char *>(yuv), *image_, width_, height_);
        else
            yuvN21toRGBRotated(static_cast<const unsigned char *>(yuv), *image_, width_, height_);
    });
    dirty_ = true;
    lck.unlock();
    cv_.notify_one();
}


void EmoDetector::analyzeCurrentFrame() {
    VisageSDK::FaceData faceData;
    while(true) {
        std::unique_lock lck{mtx_};
        if(!dirty_)
            cv_.wait(lck, [this] { return bool{dirty_}; });

        if(end_) return;
        auto* statuses = tracker_.track(image_->width, image_->height, image_->imageData, &faceData, VISAGE_FRAMEGRABBER_FMT_RGB, VISAGE_FRAMEGRABBER_ORIGIN_TL, 0, -1, 1);
        if(statuses[0] == TRACK_STAT_OK) {
            std::array<float, 7> prob;
            if(analyzer_.estimateEmotion(image_, &faceData, prob.data())) {
                if(auto emo = estimateEmotion(prob.data())) {
                    Locator::FaceBus::ref().enqueue({*emo});
                }
            }
        }
        dirty_ = false;
    }
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


void EmoDetector::yuvN21toRGB(const unsigned char* yuv, VsImage& buff, int width, int height) {
    const int frameSize = width * height;

    const int ii = 0;
    const int ij = 0;
    const int di = +1;
    const int dj = +1;

    unsigned char* rgb = (unsigned char*)buff.imageData;

    for (int i = 0, ci = ii; i < height; ++i, ci += di)
    {
        for (int j = 0, cj = ij; j < width; ++j, cj += dj)
        {
            int y = (0xff & ((int) yuv[ci * width + cj]));
            int v = (0xff & ((int) yuv[frameSize + (ci >> 1) * width + (cj & ~1) + 0]));
            int u = (0xff & ((int) yuv[frameSize + (ci >> 1) * width + (cj & ~1) + 1]));
            y = y < 16 ? 16 : y;

            int a0 = 1192 * (y -  16);
            int a1 = 1634 * (v - 128);
            int a2 =  832 * (v - 128);
            int a3 =  400 * (u - 128);
            int a4 = 2066 * (u - 128);

            int r = (a0 + a1) >> 10;
            int g = (a0 - a2 - a3) >> 10;
            int b = (a0 + a4) >> 10;

            *rgb++ = clamp(r);
            *rgb++ = clamp(g);
            *rgb++ = clamp(b);
        }
    }
}


void EmoDetector::yuvN21toRGBRotated(const unsigned char* yuv, VsImage& buff, int width, int height) {
    const int frameSize = width * height;

    const int ii = 0;
    const int ij = 0;
    const int di = +1;
    const int dj = +1;

    unsigned char* rgb = (unsigned char*)buff.imageData;

    for (int i = 0, ci = ii; i < height; ++i, ci += di)
    {
        for (int j = 0, cj = ij; j < width; ++j, cj += dj)
        {
            int y = (0xff & ((int) yuv[ci * width + cj]));
            int v = (0xff & ((int) yuv[frameSize + (ci >> 1) * width + (cj & ~1) + 0]));
            int u = (0xff & ((int) yuv[frameSize + (ci >> 1) * width + (cj & ~1) + 1]));
            y = y < 16 ? 16 : y;

            int a0 = 1192 * (y -  16);
            int a1 = 1634 * (v - 128);
            int a2 =  832 * (v - 128);
            int a3 =  400 * (u - 128);
            int a4 = 2066 * (u - 128);

            int r = (a0 + a1) >> 10;
            int g = (a0 - a2 - a3) >> 10;
            int b = (a0 + a4) >> 10;

            auto pos = 3 * ((width - 1 - cj)* height + ci);
            rgb[pos++] = clamp(r);
            rgb[pos++] = clamp(g);
            rgb[pos++] = clamp(b);
        }
    }
}


int EmoDetector::clamp(int x) {
    unsigned y;
    return !(y=x>>8) ? x : (0xff ^ (y>>24));
}


} // namespace gamee
