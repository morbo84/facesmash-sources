#ifndef FACE_SMASH_SERVICE_CAMERA_SERVICE_H
#define FACE_SMASH_SERVICE_CAMERA_SERVICE_H


namespace gamee {


struct CameraService {
    virtual ~CameraService() = default;

    virtual void init() = 0;
    virtual void release() = 0;

    virtual bool available() const noexcept = 0;
    virtual bool dirty() const noexcept = 0;

    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;

    virtual const void * pixels() const noexcept = 0;
    virtual int pitch() const noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_CAMERA_SERVICE_H
