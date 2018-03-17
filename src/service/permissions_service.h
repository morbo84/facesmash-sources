#ifndef FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H
#define FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H


#include "../common/types.h"


namespace gamee {


struct PermissionsService {
    virtual ~PermissionsService() = default;
    virtual PermissionStatus status(PermissionType) const noexcept = 0;
    virtual void request(PermissionType) noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H
