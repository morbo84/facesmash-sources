#ifndef FACE_SMASH_SERVICE_HAPTIC_NULL_H
#define FACE_SMASH_SERVICE_HAPTIC_NULL_H


#include "haptic_service.h"


namespace gamee {


struct HapticNull: HapticService {
    void rumble(RumbleEffect) override;
    void pause() override;
    void unpause() override;
};


}


#endif // FACE_SMASH_SERVICE_HAPTIC_SERVICE_H
