#ifndef FACE_SMASH_SYSTEM_MOVEMENT_SYSTEM_H
#define FACE_SMASH_SYSTEM_MOVEMENT_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct MovementSystem final {
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_MOVEMENT_SYSTEM_H
