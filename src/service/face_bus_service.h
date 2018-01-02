#ifndef FACE_SMASH_SERVICE_FACE_BUS_SERVICE_H
#define FACE_SMASH_SERVICE_FACE_BUS_SERVICE_H


#include <mutex>
#include "../common/types.h"


namespace gamee {


class FaceBusService {
    static constexpr unsigned int max = 3;

public:
    void enqueue(FaceType);
    void dequeue();

private:
    std::mutex mutex;
    FaceType faces[max];
    unsigned int pos{0u};
};


}


#endif // FACE_SMASH_SERVICE_FACE_BUS_SERVICE_H
