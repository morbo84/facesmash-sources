#ifndef FACE_SMASH_SERVICE_CAMERA_ANDROID_H
#define FACE_SMASH_SERVICE_CAMERA_ANDROID_H


#include <mutex>
#include <memory>
#include "../common/constants.h"
#include "camera_service.h"


namespace gamee {


struct CameraAndroid final: CameraService {
    CameraAndroid();
    ~CameraAndroid();

    void start() override;
    void stop() override;

    int width() const noexcept override;
    int height() const noexcept override;
    void frame(std::function<void(const void *, int)>) const noexcept override;

    void setPixels(const void*) noexcept;
    void receive(const PermissionEvent &) noexcept;

private:
    void init() noexcept;
    void swapPtrs() noexcept;

    std::unique_ptr<char[]> frame0_;
    std::unique_ptr<char[]> frame1_;
    char* p0_;
    char* p1_;
    mutable std::mutex frameMtx_;
    int width_{logicalWidth/2};
    int height_{logicalHeight/2};
    size_t size_{0};
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_ANDROID_H
