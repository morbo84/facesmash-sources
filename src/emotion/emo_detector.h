#ifndef FACE_SMASH_EMOTION_EMO_DETECTOR_H
#define FACE_SMASH_EMOTION_EMO_DETECTOR_H


#include <array>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <memory>
#include <utility>
#include <SDL_timer.h>

#if _WIN64
// needed to not make clash some name used by Visage against windows.h
#define WIN32_LEAN_AND_MEAN
#endif
#include "VisageTracker.h"
#include "VisageFaceAnalyser.h"

#include "../common/types.h"
#include "../event/event.hpp"


struct SDL_Surface;
struct SDL_Thread;


namespace gamee {


class EmoDetector {
    static constexpr auto length = 7;
    static constexpr auto visageChannels = 3;
    static constexpr Uint32 interval = 200u;

public:
    EmoDetector();
    ~EmoDetector();

    void onFrameAvailable(const FrameAvailableEvent &) noexcept;
    void onCameraInit(const CameraInitEvent &) noexcept;

private:
    enum class Emotion {anger, disgust, fear, happiness, sadness, surprise, neutral};

    static int analyzeCurrentFrame(void *);
    static std::pair<FaceType, float> estimateEmotion(const std::array<float, length> &);

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
    SDL_Rect rect_{};
};


} // namespace gamee


#endif // FACE_SMASH_EMOTION_EMO_DETECTOR_H
