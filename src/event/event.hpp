#ifndef FACE_SMASH_EVENT_EVENT_HPP
#define FACE_SMASH_EVENT_EVENT_HPP


#include <SDL_rect.h>
#include "../common/types.h"


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


struct KeyboardEvent final {
    enum class Type: UInt8 {
        ESCAPE,
        BACK
    };

    const Type type;
};


struct TouchEvent final: SDL_Point {};


struct SceneChangeEvent final {
    SceneType scene;
};


struct FaceEvent final {
    FaceType type;
};


struct FrameAvailableEvent final {};


}


#endif // FACE_SMASH_EVENT_EVENT_HPP
