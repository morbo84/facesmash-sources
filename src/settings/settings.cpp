#include "settings.h"


namespace gamee {


namespace details {


// target screen size
static constexpr UInt16 logicalWidth = 1080;
static constexpr UInt16 logicalHeight = 1920;


}


const std::pair<UInt16, UInt16> Settings::logicalSz = std::make_pair(details::logicalWidth, details::logicalHeight);


#ifdef DEBUG
bool Settings::debug() const noexcept { return true; }
#else
bool Settings::debug() const noexcept { return false; }
#endif


std::pair<UInt16, UInt16> Settings::logicalSize() const noexcept {
    return logicalSz;
}


UInt16 Settings::logicalWidth() const noexcept {
    return logicalSz.first;
}


UInt16 Settings::logicalHeight() const noexcept {
    return logicalSz.second;
}


}
