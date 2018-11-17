#include "../locator/locator.hpp"
#include "face_bus_service.h"


namespace gamee {


void FaceBusService::enqueue(FaceEvent event) {
    std::lock_guard guard{mutex};

    if(posFace < max) {
        faces[posFace++] = std::move(event);
    }

    (void)guard;
}


void FaceBusService::enqueue(FrameAvailableEvent event) {
    std::lock_guard guard{mutex};

    if(posFrame < max) {
        frames[posFrame++] = event;
    }

    (void)guard;
}


void FaceBusService::dequeue() {
    std::lock_guard guard{mutex};

    while(posFace) {
        Locator::Dispatcher::ref().enqueue<FaceEvent>(std::move(faces[--posFace]));
    }

    while(posFrame) {
        Locator::Dispatcher::ref().enqueue<FrameAvailableEvent>(frames[--posFrame]);
    }

    (void)guard;
}


}
