#include "av_recorder_null.h"


namespace gamee {


void AvRecorderNull::start(int, int) {}


void AvRecorderNull::frame(const unsigned char *, delta_type) {}


void AvRecorderNull::stop() {}


bool AvRecorderNull::recording() const noexcept { return false; }


bool AvRecorderNull::ready() const noexcept { return false; }


bool AvRecorderNull::available() const noexcept { return false; }


void AvRecorderNull::exportMedia() const {}


}