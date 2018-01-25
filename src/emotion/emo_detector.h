#ifndef FACE_SMASH_EMOTION_EMO_DETECTOR_H
#define FACE_SMASH_EMOTION_EMO_DETECTOR_H

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <memory>
#include <optional>
#include "VisageTracker.h"
#include "VisageFaceAnalyser.h"
#include "../common/types.h"
#include "../event/event.hpp"


namespace gamee {


class EmoDetector {
public:
    EmoDetector(int width, int height);
    ~EmoDetector();

    void receive(const FrameAvailableEvent &) noexcept;

private:
    enum class Emotion {anger, disgust, fear, happiness, sadness, surprise, neutral};
    void analyzeCurrentFrame();
    static std::optional<FaceType> estimateEmotion(float* prob);

    static void ARGBtoRGB(const unsigned char* argb, VsImage& buff, int width, int height);
    static void ARGBtoRGBRotated(const unsigned char* argb, VsImage& buff, int width, int height);

    const int width_;
    const int height_;
    VisageSDK::VisageTracker tracker_;
    VisageSDK::VisageFaceAnalyser analyzer_;
    VsImage* image_;
    size_t internalSize_;
    std::unique_ptr<unsigned char[]> internal_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic_bool dirty_;
    std::atomic_bool end_;
    std::thread t_;
};


} // namespace gamee


#endif // FACE_SMASH_EMOTION_EMO_DETECTOR_H
