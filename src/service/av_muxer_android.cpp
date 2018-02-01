#include "av_muxer_android.h"

#ifdef __ANDROID__
#include "common/constants.h"
#include <SDL_surface.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaMuxer.h>
#include <algorithm>
#include <string>
// _linux stuff_ needed to open and close the file descriptor for the muxer
#include <fcntl.h>
#include <unistd.h>


namespace gamee {


std::string bindingVideoOutputPath();
void bindingVideoExport();


AvMuxerAndroid::~AvMuxerAndroid() {
    std::unique_lock lck{mtx_};
    frame_.first = nullptr;
    lck.unlock();
    AvMuxerAndroid::stop();
    if(t_.joinable())
        t_.join();
}


void AvMuxerAndroid::start(int width, int height) {
    if(t_.joinable())
        t_.join();
    stopped_ = false;
    t_ = std::thread{&AvMuxerAndroid::recordVideo, this, width, height};
}


void AvMuxerAndroid::frame(const unsigned char* frame, delta_type delta) {
    std::unique_lock lck{mtx_};
    frame_.first = frame;
    frame_.second = delta;
    ready_ = false;
    lck.unlock();
    cv_.notify_one();
}


void AvMuxerAndroid::stop() {
    std::unique_lock lck{mtx_};
    stopped_ = true;
    lck.unlock();
    cv_.notify_one();
}


bool AvMuxerAndroid::recording() const noexcept {
    return !stopped_;
}


void AvMuxerAndroid::recordVideo(int width, int height) {
    constexpr auto outputFormat = SDL_PIXELFORMAT_NV12;
    const auto outputBufferSize = static_cast<size_t>(width * height) * 12U / 8U;
    constexpr auto frameRate = 30;
    auto* videoMediaFormat = AMediaFormat_new();
    AMediaFormat_setString(videoMediaFormat, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_WIDTH, width);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_HEIGHT, height);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_BIT_RATE, 2500000);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_FRAME_RATE, frameRate);
    AMediaFormat_setInt32(videoMediaFormat, "capture-rate", frameRate);
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_COLOR_FORMAT, 0x7f420888); // COLOR_FormatYUV420Flexible
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 3);

    auto* encoder = AMediaCodec_createEncoderByType("video/avc");
    AMediaCodec_configure(encoder, videoMediaFormat, nullptr, nullptr, AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    AMediaCodec_start(encoder);

    const auto filePath = bindingVideoOutputPath();
    auto fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    auto* muxer = AMediaMuxer_new(fd, AMEDIAMUXER_OUTPUT_FORMAT_MPEG_4);

    size_t videoTrack{};
    bool eos{false};
    while (true) {
        auto signedInputBufId = AMediaCodec_dequeueInputBuffer(encoder, 100000);
        if (signedInputBufId >= 0) {
            auto inputBuffer = static_cast<size_t>(signedInputBufId);
            size_t size;
            auto* buf = AMediaCodec_getInputBuffer(encoder, inputBuffer, &size);

            std::unique_lock lck{mtx_};
            cv_.wait(lck, [this] { return stopped_ || !ready_; });
            lck.unlock();

            if (!ready_) {
                SDL_ConvertPixels(width, height, internalFormat, frame_.first,
                                  width * SDL_BYTESPERPIXEL(internalFormat), outputFormat, buf,
                                  width * SDL_BYTESPERPIXEL(outputFormat));
                AMediaCodec_queueInputBuffer(encoder, inputBuffer, 0, outputBufferSize,
                                             frame_.second * 1000, 0);
            } else if (stopped_ && !eos) {
                eos = true;
                AMediaCodec_queueInputBuffer(encoder, inputBuffer, 0, 0,
                                             frame_.second * 1000,
                                             AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
            }
        } else if (stopped_ && frame_.first == nullptr)
            eos = true;

        frame_.first = nullptr;
        ready_ = true;

        AMediaCodecBufferInfo info;
        auto signedOutputBufId = AMediaCodec_dequeueOutputBuffer(encoder, &info, 100000);
        if(signedOutputBufId == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
            // here the encoder want to add a new track to the muxer
            auto* outFmt = AMediaCodec_getOutputFormat(encoder);
            auto res = AMediaMuxer_addTrack(muxer, outFmt);
            if(res < 0) {
                stopped_ = true;
                break;
            }
            videoTrack = static_cast<size_t>(res);
            AMediaMuxer_start(muxer);
        }
        else if(signedOutputBufId >= 0) {
            auto outputBuffer = static_cast<size_t>(signedOutputBufId);
            size_t size;
            const auto* buf = AMediaCodec_getOutputBuffer(encoder, outputBuffer, &size);
            AMediaMuxer_writeSampleData(muxer, videoTrack, buf, &info);
            AMediaCodec_releaseOutputBuffer(encoder, outputBuffer, false);
        } else if (eos) {
            break;
        }

    }

    AMediaMuxer_stop(muxer);
    AMediaMuxer_delete(muxer);
    close(fd);
    AMediaCodec_stop(encoder);
    AMediaCodec_delete(encoder);
    AMediaFormat_delete(videoMediaFormat);
}


bool AvMuxerAndroid::ready() const noexcept {
    return ready_;
}


bool AvMuxerAndroid::available() const noexcept {
    return true;
}


void AvMuxerAndroid::exportMedia() const {
    bindingVideoExport();
}


} // namespace gamee

#else

namespace gamee {


AvMuxerAndroid::~AvMuxerAndroid() {}


void AvMuxerAndroid::start(int width, int height) {}


void AvMuxerAndroid::frame(const unsigned char* frame, delta_type delta) {}


void AvMuxerAndroid::stop() {}


bool AvMuxerAndroid::recording() const noexcept { return false; }


void AvMuxerAndroid::recordVideo(int width, int height) {}


bool AvMuxerAndroid::ready() const noexcept { return false; }


bool AvMuxerAndroid::available() const noexcept { return false; }


void AvMuxerAndroid::export() const {}


} // namespace gamee

#endif
