#ifndef FACE_SMASH_SERVICE_FACE_BUS_SERVICE_H
#define FACE_SMASH_SERVICE_FACE_BUS_SERVICE_H


#include <mutex>
#include "../event/event.hpp"


namespace gamee {


class FaceBusService {
    static constexpr unsigned int max = 3;

public:
    void enqueue(FaceEvent);
    void enqueue(FrameAvailableEvent);
    void dequeue();

private:
    std::mutex mutex;
    FaceEvent faces[max];
    FrameAvailableEvent frames[max];
    unsigned int posFace{0u};
    unsigned int posFrame{0u};
};


}


#endif // FACE_SMASH_SERVICE_FACE_BUS_SERVICE_H
