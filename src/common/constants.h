#ifndef FACE_SMASH_COMMON_CONSTANTS_H
#define FACE_SMASH_COMMON_CONSTANTS_H


#include <SDL_pixels.h>
#include <SDL_rect.h>
#include "types.h"

#if _WIN32 || (__linux__ && !__ANDROID__) || (__APPLE__ && __MACH__)
#define FACESMASH_DESKTOP
#endif


namespace gamee {


constexpr int logicalWidth = 720;
constexpr int logicalHeight = 1280;
const SDL_Rect logicalScreen = { 0, 0, logicalWidth, logicalHeight };

constexpr int recordingWidth = 720;
constexpr int recordingHeight = 1280;

constexpr int detectorWidth = 216;
constexpr int detectorHeight = 384;

constexpr int opponentFrameWidth = 144;
constexpr int opponentFrameHeight = 256;
constexpr int opponentFrameRenderingWidth = 216;
constexpr int opponentFrameRenderingHeight = 384;

constexpr Uint32 internalFormat = SDL_PIXELFORMAT_RGBA8888;
constexpr Uint32 cameraFormat = SDL_PIXELFORMAT_NV21;
constexpr Uint32 detectorFormat = internalFormat;

const SDL_Rect playArea = { logicalWidth / 10, logicalHeight / 6, 4 * logicalWidth / 5, 2 * logicalHeight / 3 };
const SDL_Rect trainingArea = { 0, logicalHeight / 6, logicalWidth, 4 * logicalHeight / 7 };

constexpr int numberOfItems = 6;
constexpr int numberOfFaces = 6;

constexpr auto probabilityThreshold = .7f;

constexpr delta_type shakeDuration = 250_ui32;
constexpr delta_type gameDuration = 30999_ui32;
constexpr delta_type trainingDuration = 10999_ui32;

constexpr SDL_Color colorWhite{233_ui8, 241_ui8, 223_ui8, 255_ui8};
constexpr SDL_Color colorBlack{39_ui8, 39_ui8, 39_ui8, 255_ui8};
constexpr SDL_Color colorRed{229_ui8, 0_ui8, 20_ui8, 255_ui8};
constexpr SDL_Color colorYellow{255_ui8, 206_ui8, 41_ui8, 255_ui8};
constexpr SDL_Color colorBlue{0_ui8, 222_ui8, 255_ui8, 255_ui8};
constexpr SDL_Color colorGreen{0_ui8, 123_ui8, 74_ui8, 255_ui8};

constexpr auto settingsRatingState = "rating/state";
constexpr int64_t ratingDone = 0, ratingLater = 1, ratingNo = 2;


}


#endif // FACE_SMASH_COMMON_CONSTANTS_H
