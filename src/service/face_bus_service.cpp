#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "face_bus_service.h"


namespace gamee {


void FaceBusService::enqueue(FaceType type) {
    std::lock_guard guard{mutex};

    if(pos < max) {
        faces[pos++] = type;
    }

    (void)guard;
}


void FaceBusService::dequeue() {
    std::lock_guard guard{mutex};

    while(pos) {
        Locator::Dispatcher::ref().enqueue<FaceEvent>(faces[--pos]);
    }

    (void)guard;
}


}
