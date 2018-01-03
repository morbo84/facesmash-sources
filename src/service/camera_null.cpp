#include "camera_null.h"


namespace gamee {


int CameraNull::width() const noexcept {
    return 0;
}


int CameraNull::height() const noexcept {
    return 0;
}


void CameraNull::frame(std::function<void(const void *, int)> func) const noexcept {
    func(nullptr, 0);
}


}
