#ifndef FACE_SMASH_EVENT_EVENT_HPP
#define FACE_SMASH_EVENT_EVENT_HPP


#include "../types/types.hpp"


namespace gamee {


struct EnvEvent final {
    enum class Type: UInt8 {
        TERMINATING,
        LOW_MEMORY,
        ENTERING_BACKGROUND,
        ENTERED_BACKGROUND,
        ENTERING_FOREGROUND,
        ENTERED_FOREGROUND
    };

    const Type type;
};


struct TouchEvent final {
    const float x;
    const float y;
};


struct SceneEvent final {
    enum class Type: UInt8 {
        SPLASH,
        MENU,
        GAME
    };

    const Type type;
};


}


#endif // FACE_SMASH_EVENT_EVENT_HPP
