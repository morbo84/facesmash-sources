#ifndef FACE_SMASH_SERVICE_AV_RECORDER_SERVICE_H
#define FACE_SMASH_SERVICE_AV_RECORDER_SERVICE_H


#include <memory>
#include "../common/types.h"


namespace gamee {


struct AvRecorderService {
    virtual ~AvRecorderService() = default;

    virtual void start(int, int) = 0;
    virtual void frame(const unsigned char *, delta_type) = 0;
    virtual void stop() = 0;
    virtual bool recording() const noexcept = 0;
    virtual bool ready() const noexcept = 0;

    virtual bool supportExport() const = 0;
    virtual void exportMedia() = 0;
};


}


#endif // FACE_SMASH_SERVICE_AV_RECORDER_SERVICE_H
