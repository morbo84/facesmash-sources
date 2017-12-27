#ifndef FACE_SMASH_COMMON_CONSTANTS_H
#define FACE_SMASH_COMMON_CONSTANTS_H


#include <SDL_rect.h>
#include "types.h"


namespace gamee {


constexpr int logicalWidth = 1080;
constexpr int logicalHeight = 1920;
constexpr SDL_Rect logicalScreen = { 0, 0, logicalWidth, logicalHeight };


}


#endif // FACE_SMASH_COMMON_CONSTANTS_H
