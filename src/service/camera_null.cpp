#include "../common/constants.h"
#include "camera_null.h"


namespace gamee {


void CameraNull::start() {}


void CameraNull::stop() {}


int CameraNull::width() const noexcept {
    // useful for test purposes
    return logicalWidth / 2;
}


int CameraNull::height() const noexcept {
    // useful for test purposes
    return logicalHeight / 2;
}


void CameraNull::frame(std::function<void(const void *, int)>) const noexcept {
    // does nothing to avoid crashes :-)
}


}
