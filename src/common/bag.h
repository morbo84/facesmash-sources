#ifndef FACE_SMASH_COMMON_BAG_H
#define FACE_SMASH_COMMON_BAG_H


#include <random>
#include "constants.h"
#include "types.h"


namespace gamee {


struct FaceBag final {
    FaceBag();

    FaceType get();

private:
    std::mt19937 generator;
    FaceType buffer[numberOfFaces];
    Uint8 next;
};


struct ItemBag final {
    ItemBag();

    ItemType get();

private:
    std::mt19937 generator;
    ItemType buffer[numberOfItems];
    Uint8 next;
};


}


#endif // FACE_SMASH_COMMON_BAG_H
