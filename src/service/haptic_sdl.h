#ifndef FACE_SMASH_SERVICE_HAPTIC_SDL_H
#define FACE_SMASH_SERVICE_HAPTIC_SDL_H


#include <SDL_haptic.h>
#include "haptic_service.h"


namespace gamee {


class HapticSDL: public HapticService {
    static constexpr delta_type duration = 500_ui32;

public:
    HapticSDL();
    ~HapticSDL();

    void rumble(RumbleEffect) override;
    void pause() override;
    void unpause() override;

private:
    SDL_Haptic *haptic;
};


}


#endif // FACE_SMASH_SERVICE_HAPTIC_SDL_H
