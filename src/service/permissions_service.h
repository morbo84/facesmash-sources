#ifndef FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H
#define FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H


#include "../common/types.h"


namespace gamee {


struct PermissionsService {
    virtual ~PermissionsService() = default;
    virtual void request(PermissionType) = 0;
};


}


#endif // FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H
