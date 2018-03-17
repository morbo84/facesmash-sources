#include "permissions_android.h"


namespace gamee {


PermissionStatus PermissionsAndroid::status(PermissionType) const noexcept {
    // TODO
    return PermissionStatus::GRANTED;
}


void PermissionsAndroid::request(PermissionType p) noexcept {
    // TODO
}


}
