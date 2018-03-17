#ifndef FACE_SMASH_SERVICE_PERMISSIONS_ANDROID_H
#define FACE_SMASH_SERVICE_PERMISSIONS_ANDROID_H


#include "permissions_service.h"


namespace gamee {


struct PermissionsAndroid : PermissionsService {
    PermissionStatus status(PermissionType) const noexcept override;
    void request(PermissionType) noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_PERMISSIONS_ANDROID_H
