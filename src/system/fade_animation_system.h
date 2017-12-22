#ifndef FACE_SMASH_SYSTEM_FADE_ANIMATION_SYSTEM_H
#define FACE_SMASH_SYSTEM_FADE_ANIMATION_SYSTEM_H


#include "../types/types.hpp"


namespace gamee {


struct FadeAnimationSystem final {
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_FADE_ANIMATION_SYSTEM_H
