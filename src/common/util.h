#ifndef FACE_SMASH_COMMON_UTIL_H
#define FACE_SMASH_COMMON_UTIL_H


#include <SDL_rect.h>
#include "../component/component.hpp"
#include "types.h"


namespace gamee {


int numOfDigits(int);


SDL_Point transformToPosition(const Registry &, entity_type entity, const Transform &);
SDLTextureHandle toHandle(FaceType);
SDLTextureHandle toHandle(ItemType);
SDLTextureHandle toStrHud(int);
SDLTextureHandle toStrDebug(int);


}


#endif // FACE_SMASH_COMMON_UTIL_H
