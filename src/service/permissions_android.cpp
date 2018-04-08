#include "permissions_android.h"
#include "../common/types.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"


namespace gamee {

#ifdef __ANDROID__
bool dequeuePermissionResult(std::pair<int, int>& p);
int checkPermissionStatus(int permission);
void requestPermission(int permission);
#else
bool dequeuePermissionResult(std::pair<int, int>&) { return false; }
int checkPermissionStatus(int) { return static_cast<int>(PermissionStatus::DENIED); }
void requestPermission(int) {}
#endif


PermissionStatus PermissionsAndroid::status(PermissionType p) const noexcept {
    auto status = checkPermissionStatus(static_cast<int>(p));
    return static_cast<PermissionStatus>(status);
}


void PermissionsAndroid::request(PermissionType p) noexcept {
    requestPermission(static_cast<int>(p));
}


void PermissionsAndroid::dequeue() noexcept {
    std::pair<int, int> p;
    while(dequeuePermissionResult(p)) {
        PermissionEvent event{static_cast<PermissionType>(p.first), static_cast<PermissionStatus>(p.second)};
        Locator::Dispatcher::ref().enqueue<PermissionEvent>(event);
    }
}


}
