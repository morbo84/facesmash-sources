#ifndef FACE_SMASH_EMOTION_EMO_DETECTOR_H
#define FACE_SMASH_EMOTION_EMO_DETECTOR_H

#include <memory>
#include <optional>
#include "VisageTracker.h"
#include "VisageFaceAnalyser.h"
#include "../common/types.h"
#include "../event/event.hpp"


struct SDL_Thread;
struct SDL_mutex;
struct SDL_cond;


namespace gamee {


class EmoDetector {
public:
    EmoDetector();
    ~EmoDetector();

    void receive(const FrameAvailableEvent &) noexcept;
    void receive(const CameraInitEvent &) noexcept;

private:
    enum class Emotion {anger, disgust, fear, happiness, sadness, surprise, neutral};
    void start(int width, int height);

    static int analyzeCurrentFrame(void *);
    static std::optional<FaceType> estimateEmotion(float* prob);

    static void ARGBtoRGB(const unsigned char* argb, VsImage& buff, int width, int height);
    static void ARGBtoRGBRotated(const unsigned char* argb, VsImage& buff, int width, int height);

    int width_;
    int height_;
    VisageSDK::VisageTracker tracker_;
    VisageSDK::VisageFaceAnalyser analyzer_;
    VsImage* image_;
    size_t internalSize_;
    std::unique_ptr<unsigned char[]> internal_;
    SDL_Thread *t_;
    SDL_mutex *mtx_;
    SDL_cond *cv_;
    bool end_;
};


} // namespace gamee


#endif // FACE_SMASH_EMOTION_EMO_DETECTOR_H
