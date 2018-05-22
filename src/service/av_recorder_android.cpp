#include "av_recorder_android.h"

#ifdef __ANDROID__
#include "common/constants.h"
#include <SDL_surface.h>
#include <SDL_thread.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaFormat.h>
#include <media/NdkMediaMuxer.h>
#include <media/NdkMediaExtractor.h>
#include <algorithm>
#include <array>
#include <string>
// _linux stuff_ needed to open and close the file descriptor for the muxer
#include <fcntl.h>
#include <unistd.h>


namespace gamee {


std::string bindingVideoOutputPath();
void bindingVideoExport();


AvRecorderAndroid::~AvRecorderAndroid() {
    std::unique_lock lck{mtx_};
    frame_.first = nullptr;
    lck.unlock();
    AvRecorderAndroid::stop();
    SDL_WaitThread(t_, nullptr);
    t_ = nullptr;
}


void AvRecorderAndroid::start(int w, int h) {
    SDL_WaitThread(t_, nullptr);

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
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_COLOR_FORMAT, 0x7f420888); // COLOR_FormatYUV420Flexible
    AMediaFormat_setInt32(videoMediaFormat, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 3);

    auto* encoder = AMediaCodec_createEncoderByType("video/avc");
    AMediaCodec_configure(encoder, videoMediaFormat, nullptr, nullptr, AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    AMediaCodec_start(encoder);

    const auto filePath = bindingVideoOutputPath();
    auto fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0666);
    auto* muxer = AMediaMuxer_new(fd, AMEDIAMUXER_OUTPUT_FORMAT_MPEG_4);

    auto* extractor = AMediaExtractor_new();
    AMediaExtractor_setDataSource(extractor, "/data/user/0/com.gamee.facesmash/files/audio/music_play.aac"); // TODO
    AMediaExtractor_selectTrack(extractor, 0U);
    auto* audioFormat = AMediaExtractor_getTrackFormat(extractor, 0U);
    auto audioTrack = (size_t)AMediaMuxer_addTrack(muxer, audioFormat); // TODO: cast
    AMediaExtractor_seekTo(extractor, 0, AMEDIAEXTRACTOR_SEEK_CLOSEST_SYNC);

    size_t videoTrack{};
    AMediaCodecBufferInfo videoBufInfo;
    bool videoEos{false};
    while (true) {
        auto signedInputBufId = AMediaCodec_dequeueInputBuffer(encoder, 100000);
        if (signedInputBufId >= 0) {
            auto inputBuffer = static_cast<size_t>(signedInputBufId);
            size_t size;
            auto* buf = AMediaCodec_getInputBuffer(encoder, inputBuffer, &size);

            std::unique_lock lck{recorder.mtx_};
            recorder.cv_.wait(lck, [&recorder] { return recorder.stopped_ || !recorder.ready_; });
            lck.unlock();

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
        } else if (recorder.stopped_ && recorder.frame_.first == nullptr)
            videoEos = true;

        recorder.frame_.first = nullptr;
        recorder.ready_ = true;

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
        }
        else if(signedOutputBufId >= 0) {
            auto outputBuffer = static_cast<size_t>(signedOutputBufId);
            size_t size;
            const auto* buf = AMediaCodec_getOutputBuffer(encoder, outputBuffer, &size);
            AMediaMuxer_writeSampleData(muxer, videoTrack, buf, &videoBufInfo);
            AMediaCodec_releaseOutputBuffer(encoder, outputBuffer, false);
        } else if (videoEos) {
            break;
        }
    }

    // mux the audio track
    std::array<uint8_t, 256U * 1024U> audioBuf;
    AMediaCodecBufferInfo audioBufInfo{};
    audioBufInfo.flags = AMediaExtractor_getSampleFlags(extractor);
    bool audioEos{};
    while (!audioEos) {
        audioBufInfo.size = AMediaExtractor_readSampleData(extractor, audioBuf.data(), audioBuf.size());
        if(audioBufInfo.size < 0 || audioBufInfo.presentationTimeUs >= videoBufInfo.presentationTimeUs) {
            audioEos = true;
        } else {
            audioBufInfo.presentationTimeUs = AMediaExtractor_getSampleTime(extractor);
            AMediaMuxer_writeSampleData(muxer, audioTrack, audioBuf.data(), &audioBufInfo);
            AMediaExtractor_advance(extractor);
        }
    }

    AMediaMuxer_stop(muxer);
    AMediaMuxer_delete(muxer);
    close(fd);
    AMediaCodec_stop(encoder);
    AMediaCodec_delete(encoder);
    AMediaFormat_delete(videoMediaFormat);
    AMediaFormat_delete(audioFormat);
    AMediaExtractor_delete(extractor);

    return 0;
}


bool AvRecorderAndroid::ready() const noexcept {
    return ready_;
}


bool AvRecorderAndroid::supportExport() const {
    return true;
}


void AvRecorderAndroid::exportMedia() const {
    bindingVideoExport();
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


void AvRecorderAndroid::exportMedia() const {}


} // namespace gamee


#endif
