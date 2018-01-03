#ifndef FACE_SMASH_SERVICE_CAMERA_ANDROID_H
#define FACE_SMASH_SERVICE_CAMERA_ANDROID_H


#include "camera_service.h"
#include <mutex>


namespace gamee {


struct CameraAndroid final: CameraService {
    CameraAndroid();
    void init();

    int width() const noexcept override;
    int height() const noexcept override;
    void frame(std::function<void(const void *, int)>) const noexcept override;

    void setPixels(const void*);

private:
    char* frame_{nullptr};
    mutable std::mutex frameMtx_;
    int width_{-1};
    int height_{-1};
    size_t size_{0};
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_ANDROID_H
