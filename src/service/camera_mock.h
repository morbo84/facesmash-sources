#ifndef FACE_SMASH_SERVICE_CAMERA_MOCK_H
#define FACE_SMASH_SERVICE_CAMERA_MOCK_H


#include "camera_service.h"


namespace gamee {


struct CameraMock final: CameraService {
    void init() override;
    void release() override;

    int width() const noexcept override;
    int height() const noexcept override;

    const void * frame() const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_MOCK_H
