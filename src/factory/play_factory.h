#ifndef FACE_SMASH_FACTORY_PLAY_FACTORY_H
#define FACE_SMASH_FACTORY_PLAY_FACTORY_H


#include "../common/types.h"


namespace gamee {


entity_type createExplosion(Registry &, int);
entity_type createSmashButton(Registry &, FaceType, int);
entity_type createFace(Registry &, FaceType, Uint8, Uint8, int);
entity_type createItem(Registry &, ItemType, int);


}


#endif // FACE_SMASH_FACTORY_PLAY_FACTORY_H
