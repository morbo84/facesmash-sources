#ifndef FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H


#include "../common/bag.h"
#include "../common/types.h"


namespace gamee {


class PlayFactory;


struct TheGameSystem final {
    void update(Registry &, PlayFactory &);

private:
    FaceBag faceBag;
    ItemBag itemBag;
};


}


#endif // FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
