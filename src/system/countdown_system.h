#ifndef FACE_SMASH_SYSTEM_COUNTDOWN_SYSTEM_H
#define FACE_SMASH_SYSTEM_COUNTDOWN_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct CountdownSystem final {
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_COUNTDOWN_SYSTEM_H
