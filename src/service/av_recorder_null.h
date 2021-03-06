#ifndef FACE_SMASH_SERVICE_RECORDER_NULL_H
#define FACE_SMASH_SERVICE_RECORDER_NULL_H


#include "av_recorder_service.h"


namespace gamee {


struct AvRecorderNull: AvRecorderService {
    void start(int, int) override;
    void frame(const unsigned char *, delta_type) override;
    void stop() override;
    bool recording() const noexcept override;
    bool ready() const noexcept override;
    bool supportExport() const override;
    void exportMedia() override;
};


}


#endif // FACE_SMASH_SERVICE_RECORDER_NULL_H
