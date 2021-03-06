#ifndef FACE_SMASH_SERVICE_CAMERA_SERVICE_H
#define FACE_SMASH_SERVICE_CAMERA_SERVICE_H


#include <functional>


namespace gamee {


struct CameraService {
    virtual ~CameraService() = default;

    /**
     * It must be called ONLY after the
     * camera permission has been granted.
     */
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;

    virtual void frame(std::function<void(const void *, int)>) const noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_SERVICE_H
