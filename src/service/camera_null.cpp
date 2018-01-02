#include "camera_null.h"


namespace gamee {


int CameraNull::width() const noexcept {
    return 0;
}


int CameraNull::height() const noexcept {
    return 0;
}


const void * CameraNull::pixels() const noexcept {
    return nullptr;
}


int CameraNull::pitch() const noexcept {
    return {};
}


}
