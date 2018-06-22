#include "av_recorder_android.h"

#ifdef __ANDROID__
#include "common/constants.h"
#include <SDL_surface.h>
#include <SDL_thread.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaMuxer.h>
#include <algorithm>
#include <array>
#include <string>
// _linux stuff_ needed to open and close the file descriptor for the muxer
#include <fcntl.h>
#include <unistd.h>


namespace gamee {


std::string bindingVideoOutputPath();
void bindingVideoExport();
void bindingMuxAudioVideo();


AvRecorderAndroid::~AvRecorderAndroid() {
    std::unique_lock lck{mtx_};
    frame_.first = nullptr;
    lck.unlock();
    AvRecorderAndroid::stop();
    waitRecording();
}


void AvRecorderAndroid::start(int w, int h) {
    waitRecording();

    width = w;
    height = h;

    stopped_ = false;
    t_ = SDL_CreateThread(&AvRecorderAndroid::recordVideo, "recorder", this);
}


void AvRecorderAndroid::frame(const unsigned char* frame, delta_type delta) {
    std::unique_lock lck{mtx_};
    frame_.first = frame;
    frame_.second = delta;
    ready_ = false;
    lck.unlock();
    cv_.notify_one();
}


void AvRecorderAndroid::stop() {
    std::unique_lock lck{mtx_};
    stopped_ = true;
    lck.unlock();
    cv_.notify_one();
}


bool AvRecorderAndroid::recording() const noexcept {
    return !stopped_;
}


int AvRecorderAndroid::recordVideo(void *ptr) {
    auto &recorder = *static_cast<AvRecorderAndroid *>(ptr);
    std::unique_lock lck{recorder.mtx_};

    constexpr auto outputFormat = SDL_PIXELFORMAT_NV12;
    const auto outputBufferSize = static_cast<size_t>(recorder.width * recorder.height) * 12U / 8U;
    constexpr auto frameRate = 30;
    auto* videoMediaFormat = AMediaFormat_new();
    AMediaFormat_setString(videoMediaFormat, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_WIDTH, recorder.width);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_HEIGHT, recorder.height);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_BIT_RATE, 2500000);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_FRAME_RATE, frameRate);
    AMediaFormat_setInt32(videoMediaFormat, "capture-rate", frameRate);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 3);
    /**
     * NOTE: the 0x15 value (COLOR_FormatYUV420PackedSemiPlanar) is deprecated;
     * instead, we should use COLOR_FormatYUV420Flexible (0x7f420888), because the Media Codec
     * documentations states that "All video codecs support flexible YUV 4:2:0 buffers since LOLLIPOP_MR1".
     * But flexibility comes at a cost:  we cannot impose Android anymore what color format to
     * accept, so it will be free to pick a format from the YUV420 set.
     *
     * For now, we try to force 0x15 (NV12, actually) and see whether someone complains.
     * In the future, we might try to query the videoMediaFormat for the actual color format used
     * and use the corresponding SDL pixelformat.
     */
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_COLOR_FORMAT, 0x15);

    auto* encoder = AMediaCodec_createEncoderByType("video/avc");
    AMediaCodec_configure(encoder, videoMediaFormat, nullptr, nullptr, AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    AMediaCodec_start(encoder);

    const auto filePath = bindingVideoOutputPath();
    auto fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    auto* muxer = AMediaMuxer_new(fd, AMEDIAMUXER_OUTPUT_FORMAT_MPEG_4);

    size_t videoTrack{};
    bool videoEos{false};

    while (true) {
        auto signedInputBufId = AMediaCodec_dequeueInputBuffer(encoder, 100000);

        if (signedInputBufId >= 0) {
            auto inputBuffer = static_cast<size_t>(signedInputBufId);
            size_t size;
            auto* buf = AMediaCodec_getInputBuffer(encoder, inputBuffer, &size);

            recorder.cv_.wait(lck, [&recorder] { return recorder.stopped_ || !recorder.ready_; });

            if (!recorder.ready_) {
                SDL_ConvertPixels(recorder.width, recorder.height, internalFormat, recorder.frame_.first,
                                  recorder.width * SDL_BYTESPERPIXEL(internalFormat), outputFormat, buf,
                                  recorder.width * SDL_BYTESPERPIXEL(outputFormat));
                AMediaCodec_queueInputBuffer(encoder, inputBuffer, 0, outputBufferSize,
                                             recorder.frame_.second * 1000, 0);
            } else if (recorder.stopped_ && !videoEos) {
                videoEos = true;
                AMediaCodec_queueInputBuffer(encoder, inputBuffer, 0, 0,
                                             recorder.frame_.second * 1000,
                                             AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            }
        } else if (recorder.stopped_ && recorder.frame_.first == nullptr) {
            videoEos = true;
        }

        recorder.frame_.first = nullptr;
        recorder.ready_ = true;

        AMediaCodecBufferInfo videoBufInfo;
        auto signedOutputBufId = AMediaCodec_dequeueOutputBuffer(encoder, &videoBufInfo, 100000);
        if(signedOutputBufId == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
            // here the encoder want to add a new track to the muxer
            auto* outFmt = AMediaCodec_getOutputFormat(encoder);
            auto res = AMediaMuxer_addTrack(muxer, outFmt);
            if(res < 0) {
                recorder.stopped_ = true;
                break;
            }
            videoTrack = static_cast<size_t>(res);
            AMediaMuxer_start(muxer);
        } else if(signedOutputBufId >= 0) {
            auto outputBuffer = static_cast<size_t>(signedOutputBufId);
            size_t size;
            const auto* buf = AMediaCodec_getOutputBuffer(encoder, outputBuffer, &size);
            AMediaMuxer_writeSampleData(muxer, videoTrack, buf, &videoBufInfo);
            AMediaCodec_releaseOutputBuffer(encoder, outputBuffer, false);
        } else if (videoEos) {
            break;
        }
    }

    lck.unlock();

    AMediaMuxer_stop(muxer);
    AMediaMuxer_delete(muxer);
    close(fd);
    AMediaCodec_stop(encoder);
    AMediaCodec_delete(encoder);
    AMediaFormat_delete(videoMediaFormat);

    bindingMuxAudioVideo();

    return 0;
}


bool AvRecorderAndroid::ready() const noexcept {
    return ready_;
}


bool AvRecorderAndroid::supportExport() const {
    return true;
}


void AvRecorderAndroid::exportMedia() {
    stop();
    waitRecording();
    bindingVideoExport();
}


void AvRecorderAndroid::waitRecording() {
    SDL_WaitThread(t_, nullptr);
    t_ = nullptr;
}


} // namespace gamee


#else


namespace gamee {


AvRecorderAndroid::~AvRecorderAndroid() {}


void AvRecorderAndroid::start(int width, int height) {}


void AvRecorderAndroid::frame(const unsigned char* frame, delta_type delta) {}


void AvRecorderAndroid::stop() {}


bool AvRecorderAndroid::recording() const noexcept { return false; }


int AvRecorderAndroid::recordVideo(void *) { return {}; }


bool AvRecorderAndroid::ready() const noexcept { return false; }


bool AvRecorderAndroid::supportExport() const { return false; }


void AvRecorderAndroid::exportMedia() {}


} // namespace gamee


#endif
