#include "av_muxer_null.h"


namespace gamee {


void AvMuxerNull::start(int, int) {}


void AvMuxerNull::frame(void*) {}


void AvMuxerNull::stop() {}


bool AvMuxerNull::isRecording() const noexcept { return false; }


std::string AvMuxerNull::filePath() const noexcept { return {}; }


}
