#ifndef FACE_SMASH_COMMON_CONSTANTS_H
#define FACE_SMASH_COMMON_CONSTANTS_H


#include <SDL_pixels.h>
#include <SDL_rect.h>
#include "types.h"


namespace gamee {


constexpr int logicalWidth = 1080;
constexpr int logicalHeight = 1920;
constexpr SDL_Rect logicalScreen = { 0, 0, logicalWidth, logicalHeight };
constexpr SDL_Rect smashArea = { logicalWidth / 8, logicalHeight / 8, 6 * logicalWidth / 8, 6 * logicalHeight / 8 };

constexpr int numberOfItems = 3;
constexpr int numberOfFaces = 6;


constexpr SDL_Color colorWhite{241_ui8, 231_ui8, 222_ui8, 255_ui8};
constexpr SDL_Color colorBlack{52_ui8, 61_ui8, 58_ui8, 255_ui8};
constexpr SDL_Color colorRed{229_ui8, 0_ui8, 20_ui8, 255_ui8};
constexpr SDL_Color colorYellow{255_ui8, 206_ui8, 41_ui8, 255_ui8};
constexpr SDL_Color colorBlue{0_ui8, 92_ui8, 169_ui8, 255_ui8};
constexpr SDL_Color colorLightBlue{0_ui8, 222_ui8, 255_ui8, 255_ui8};
constexpr SDL_Color colorGreen{0_ui8, 123_ui8, 74_ui8, 255_ui8};


#ifdef __ANDROID__
#define CAMERA_FRAME_AVAILABLE
#endif // __ANDROID__


}


#endif // FACE_SMASH_COMMON_CONSTANTS_H
