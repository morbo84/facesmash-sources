#include "haptic_sdl.h"


namespace gamee {


HapticSDL::HapticSDL(): haptic{nullptr} {
    auto num = SDL_NumHaptics();

    if(num) {
        haptic = SDL_HapticOpen(0);

        if(haptic) {
            if(!SDL_HapticRumbleSupported(haptic) || SDL_HapticRumbleInit(haptic)) {
                SDL_HapticClose(haptic);
                haptic = nullptr;
            }
        }
    }
}


HapticSDL::~HapticSDL() {
    if(haptic) {
        SDL_HapticRumbleStop(haptic);
        SDL_HapticClose(haptic);
    }
}


void HapticSDL::rumble(RumbleEffect effect) {
    if(haptic) {
        switch(effect) {
        case RumbleEffect::SUPER_SUPER_SOFT:
            SDL_HapticRumblePlay(haptic, .1f, minimal);
            break;
        case RumbleEffect::SUPER_SOFT:
            SDL_HapticRumblePlay(haptic, .2f, standard);
            break;
        case RumbleEffect::SOFT:
            SDL_HapticRumblePlay(haptic, .4f, standard);
            break;
        case RumbleEffect::NORMAL:
            SDL_HapticRumblePlay(haptic, .6f, standard);
            break;
        case RumbleEffect::HARD:
            SDL_HapticRumblePlay(haptic, .8f, standard);
            break;
        case RumbleEffect::SUPER_HARD:
            SDL_HapticRumblePlay(haptic, 1.f, standard);
            break;
        }
    }
}


void HapticSDL::pause() {
    if(haptic && (SDL_HapticQuery(haptic) & SDL_HAPTIC_PAUSE)) {
        SDL_HapticPause(haptic);
    }
}


void HapticSDL::unpause() {
    if(haptic && (SDL_HapticQuery(haptic) & SDL_HAPTIC_PAUSE)) {
        SDL_HapticUnpause(haptic);
    }
}


}
