#ifndef FACE_SMASH_SERVICE_HAPTIC_SERVICE_H
#define FACE_SMASH_SERVICE_HAPTIC_SERVICE_H


#include "../common/types.h"


namespace gamee {


enum class RumbleEffect: Uint8 {
    SUPER_SOFT,
    SOFT,
    NORMAL,
    HARD,
    SUPER_HARD
};


struct HapticService {
    virtual ~HapticService() = default;

    virtual void rumble(RumbleEffect) = 0;
    virtual void pause() = 0;
    virtual void unpause() = 0;
};


}


#endif // FACE_SMASH_SERVICE_HAPTIC_SERVICE_H
