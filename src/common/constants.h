#ifndef FACE_SMASH_COMMON_CONSTANTS_H
#define FACE_SMASH_COMMON_CONSTANTS_H


#include <SDL_rect.h>
#include "types.h"


namespace gamee {


constexpr int logicalWidth = 1080;
constexpr int logicalHeight = 1920;
constexpr SDL_Rect logicalScreen = { 0, 0, logicalWidth, logicalHeight };
constexpr SDL_Rect smashArea = { logicalWidth / 8, logicalHeight / 8, 6 * logicalWidth / 8, 6 * logicalHeight / 8 };

constexpr int numberOfItems = 3;
constexpr int numberOfFaces = 6;


#ifdef __ANDROID__
#define CAMERA_FRAME_AVAILABLE
#endif // __ANDROID__


}


#endif // FACE_SMASH_COMMON_CONSTANTS_H
