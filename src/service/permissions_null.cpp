#include "permissions_null.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"


namespace gamee {


void PermissionsNull::request(PermissionType p) {
    Locator::Dispatcher::ref().enqueue<PermissionEvent>(p, PermissionResultType::GRANTED);
}


}
