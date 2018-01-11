#ifndef FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class Factory;


struct TheGameSystem final {
    void update(Registry &, Factory &);
};


}


#endif // FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
