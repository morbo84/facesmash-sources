#ifndef FACE_SMASH_SERVICE_PERMISSIONS_NULL_H
#define FACE_SMASH_SERVICE_PERMISSIONS_NULL_H


#include "permissions_service.h"


namespace gamee {


struct PermissionsNull : PermissionsService {
    PermissionStatus status(PermissionType) const noexcept override;
    void request(PermissionType) noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_PERMISSIONS_NULL_H
