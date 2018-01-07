#ifndef FACE_SMASH_COMMON_UTIL_H
#define FACE_SMASH_COMMON_UTIL_H


#include <SDL_rect.h>
#include "../component/component.hpp"
#include "types.h"


namespace gamee {


struct Transform;


SDL_Point transformToPosition(const Registry &, entity_type entity, const Transform &);


}


#endif // FACE_SMASH_COMMON_UTIL_H
