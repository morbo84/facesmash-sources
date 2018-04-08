#ifndef FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H
#define FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H


#include <SDL_stdinc.h>


namespace gamee {


enum class PermissionType: Uint8;
enum class PermissionStatus: Uint8;


struct PermissionsService {
    virtual ~PermissionsService() = default;
    virtual PermissionStatus status(PermissionType) const noexcept = 0;
    virtual void request(PermissionType) noexcept = 0;
    virtual void dequeue() noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_PERMISSIONS_SERVICE_H
