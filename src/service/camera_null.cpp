#include "camera_null.h"


namespace gamee {


void CameraNull::init() {}
void CameraNull::release() {}


bool CameraNull::available() const noexcept {
    return false;
}


bool CameraNull::dirty() const noexcept {
    return false;
}


int CameraNull::width() const noexcept {
    return {};
}


int CameraNull::height() const noexcept {
    return {};
}


const void * CameraNull::pixels() const noexcept {
    return nullptr;
}


int CameraNull::pitch() const noexcept {
    return {};
}


}
