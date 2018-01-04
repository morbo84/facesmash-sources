#ifndef FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct TheGameSystem final {
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
