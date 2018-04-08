#include "permissions_null.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"


namespace gamee {


PermissionStatus PermissionsNull::status(PermissionType) const noexcept {
    return PermissionStatus::GRANTED;
}


void PermissionsNull::request(PermissionType p) noexcept {
    Locator::Dispatcher::ref().enqueue<PermissionEvent>(p, PermissionStatus::GRANTED);
}


void PermissionsNull::dequeue() noexcept {}


}
