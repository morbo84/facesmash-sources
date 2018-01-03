#ifndef FACE_SMASH_SERVICE_CAMERA_ANDROID_H
#define FACE_SMASH_SERVICE_CAMERA_ANDROID_H


#include "camera_service.h"


namespace gamee {


struct CameraAndroid final: CameraService {
    int width() const noexcept override;
    int height() const noexcept override;

    void frame(std::function<void(const void *, int)>) const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_ANDROID_H
