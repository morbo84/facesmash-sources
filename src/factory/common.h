#ifndef FACE_SMASH_FACTORY_COMMON_H
#define FACE_SMASH_FACTORY_COMMON_H


#include "../common/types.h"


namespace gamee {


entity_type createSprite(Registry &, entity_type, SDLTextureHandle, int);
entity_type createSprite(Registry &, SDLTextureHandle, int);
entity_type createFaceBlueprint(Registry &, FaceType, int);
entity_type createItemBlueprint(Registry &, ItemType, int);

void setPos(Registry &, entity_type, float, float);
void setSpriteGeometry(Registry &, entity_type, int, int, int, int, Uint8, Uint8);
void setSpriteSize(Registry &, entity_type, int, int);
void setBoundingBox(Registry &, entity_type, int, int, int, int);
void setBoundingBox(Registry &, entity_type, int, int);


}


#endif // FACE_SMASH_FACTORY_COMMON_H
