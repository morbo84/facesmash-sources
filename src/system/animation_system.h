#ifndef FACE_SMASH_SYSTEM_ANIMATION_SYSTEM_H
#define FACE_SMASH_SYSTEM_ANIMATION_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class AnimationSystem final {
    void fadeAnimation(Registry &, delta_type);
    void rotationAnimation(Registry &, delta_type);
    void spriteAnimation(Registry &, delta_type);
    void horizontalAnimation(Registry &, delta_type);
    void verticalAnimation(Registry &, delta_type);

public:
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_ANIMATION_SYSTEM_H
