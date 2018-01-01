#ifndef FACE_SMASH_EVENT_EVENT_HPP
#define FACE_SMASH_EVENT_EVENT_HPP


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


struct TouchEvent final {
    const float x;
    const float y;
};


struct FaceEvent final {
    FaceType type;
};


struct FrameEvent final {};


struct ScoreEvent final {
    int score;
};


struct SmashEvent final {
    UInt16 smash;
    UInt16 miss;
};


}


#endif // FACE_SMASH_EVENT_EVENT_HPP
