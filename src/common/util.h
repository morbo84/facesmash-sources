#ifndef FACE_SMASH_COMMON_UTIL_H
#define FACE_SMASH_COMMON_UTIL_H


#include <SDL_rect.h>
#include "types.h"


namespace gamee {


struct Transform;


int numOfDigits(int);


SDL_Point transformToPosition(const Registry &, entity_type entity, const Transform &);
SDLTextureHandle toStrHud(int);
SDLTextureHandle toStrDebug(int);


}


#endif // FACE_SMASH_COMMON_UTIL_H
