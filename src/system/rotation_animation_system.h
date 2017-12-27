#ifndef FACE_SMASH_SYSTEM_ROTATION_ANIMATION_SYSTEM_H
#define FACE_SMASH_SYSTEM_ROTATION_ANIMATION_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct RotationAnimationSystem final {
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_ROTATION_ANIMATION_SYSTEM_H
