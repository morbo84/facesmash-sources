#ifndef FACE_SMASH_SETTINGS_SETTINGS_H
#define FACE_SMASH_SETTINGS_SETTINGS_H


#include <array>
#include <utility>
#include "../types/types.hpp"


namespace gamee {


struct Settings final {
    bool debug() const noexcept;

    std::pair<UInt16, UInt16> logicalSize() const noexcept;
    UInt16 logicalWidth() const noexcept;
    UInt16 logicalHeight() const noexcept;

private:
    static const std::pair<UInt16, UInt16> logicalSz;
};


}


#endif // FACE_SMASH_SETTINGS_SETTINGS_H
