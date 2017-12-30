#ifndef FACE_SMASH_SERVICE_CAMERA_NULL_H
#define FACE_SMASH_SERVICE_CAMERA_NULL_H


#include "camera_service.h"


namespace gamee {


struct CameraNull final: CameraService {
    void init() override;
    void release() override;

    bool available() const noexcept override;

    int width() const noexcept override;
    int height() const noexcept override;

    const void * pixels() const noexcept override;
    int pitch() const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_NULL_H
