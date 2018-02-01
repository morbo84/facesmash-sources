#include "av_muxer_null.h"


namespace gamee {


void AvMuxerNull::start(int, int) {}


void AvMuxerNull::frame(const unsigned char *, delta_type) {}


void AvMuxerNull::stop() {}


bool AvMuxerNull::recording() const noexcept { return false; }


bool AvMuxerNull::ready() const noexcept { return false; }


bool AvMuxerNull::available() const noexcept { return false; }


void AvMuxerNull::exportMedia() const {}


}
