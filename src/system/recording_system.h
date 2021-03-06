#ifndef FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
#define FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H


#include <memory>
#include <functional>
#include "../common/types.h"


namespace gamee {


struct AvRecorderEvent;
struct GameRenderer;


struct RecordingSystem final {
    RecordingSystem();
    ~RecordingSystem();

    void init();

    void receive(const AvRecorderEvent &) noexcept;

    void update(GameRenderer &, delta_type, std::function<void(void)>);

private:
    std::unique_ptr<unsigned char[]> pixels;
    delta_type accumulator;
    delta_type frameTime;
    bool firstFrame;
    bool hasFrame;
    int pitch;
};


}


#endif // FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
