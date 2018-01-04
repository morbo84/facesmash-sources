#ifndef FACE_SMASH_SYSTEM_CAMERA_SYSTEM_H
#define FACE_SMASH_SYSTEM_CAMERA_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct CameraSystem final {
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_CAMERA_SYSTEM_H
