#include "camera_null.h"


namespace gamee {


int CameraNull::width() const noexcept {
    return 0;
}


int CameraNull::height() const noexcept {
    return 0;
}


void CameraNull::pixels(std::function<void(const void *)> func) const noexcept {
    func(nullptr);
}


}
