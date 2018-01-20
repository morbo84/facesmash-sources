#ifndef FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
#define FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


struct RecordingSystem final {
    void update(GameRenderer &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
