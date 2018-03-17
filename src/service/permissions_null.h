#ifndef FACE_SMASH_SERVICE_PERMISSIONS_NULL_H
#define FACE_SMASH_SERVICE_PERMISSIONS_NULL_H


#include "permissions_service.h"


namespace gamee {


struct PermissionsNull : PermissionsService {
    void request(PermissionType) override;
};


}


#endif // FACE_SMASH_SERVICE_PERMISSIONS_NULL_H
