#include "../common/constants.h"
#include "camera_null.h"


namespace gamee {


void CameraNull::start() {}


void CameraNull::stop() {}


int CameraNull::width() const noexcept {
    return logicalWidth;
}


int CameraNull::height() const noexcept {
    return logicalHeight;
}


void CameraNull::frame(std::function<void(const void *, int)> func) const noexcept {
    func(nullptr, 0);
}


}
