#include "camera_android.h"


namespace gamee {


int CameraAndroid::width() const noexcept {
    // TODO
    return {};
}


int CameraAndroid::height() const noexcept {
    // TODO
    return {};
}


void CameraAndroid::frame(std::function<void(const void *, int)> func) const noexcept {
    // TODO
    func(nullptr, 0);
}


}
