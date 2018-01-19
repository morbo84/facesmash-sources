#include "av_muxer_null.h"


namespace gamee {


void AvMuxerNull::start(int, int) {}


void AvMuxerNull::frame(std::unique_ptr<unsigned char[]>, delta_type) {}


void AvMuxerNull::stop() {}


bool AvMuxerNull::recording() const noexcept { return false; }


}
