#ifndef FACE_SMASH_EMOTION_EMO_DETECTOR_H
#define FACE_SMASH_EMOTION_EMO_DETECTOR_H


#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <memory>
#include <optional>
#include <SDL_timer.h>
#include "VisageTracker.h"
#include "VisageFaceAnalyser.h"
#include "../common/types.h"
#include "../event/event.hpp"


struct SDL_Surface;
struct SDL_Thread;


namespace gamee {


class EmoDetector {
    static constexpr auto visageChannels = 3;
    static constexpr Uint32 interval = 200u;
    static constexpr auto minProb = 0.80f;

public:
    EmoDetector();
    ~EmoDetector();

    void receive(const FrameAvailableEvent &) noexcept;
    void receive(const CameraInitEvent &) noexcept;

private:
    enum class Emotion {anger, disgust, fear, happiness, sadness, surprise, neutral};

    static int analyzeCurrentFrame(void *);
    static std::optional<FaceType> estimateEmotion(float* prob);

    Uint32 timeout;
    VisageSDK::VisageTracker tracker_;
    VisageSDK::VisageFaceAnalyser analyzer_;
    VsImage* image_;
    std::unique_ptr<unsigned char[]> frame_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::atomic_bool dirty_;
    std::atomic_bool end_;
    SDL_Thread *t_;
    SDL_Surface *copy_;
    SDL_Surface *dst_;
};


} // namespace gamee


#endif // FACE_SMASH_EMOTION_EMO_DETECTOR_H
