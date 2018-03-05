#ifndef FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
#define FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H


#include <memory>
#include <functional>
#include "../common/types.h"


namespace gamee {


struct RecorderEvent;
struct GameRenderer;


struct RecordingSystem final {
    RecordingSystem();
    ~RecordingSystem();

    void init();

    void receive(const RecorderEvent &) noexcept;

    void update(GameRenderer &, delta_type, std::function<void(void)>);

private:
    std::unique_ptr<unsigned char[]> pixels;
    delta_type accumulator;
    bool firstFrame;
    int pitch;
};


}


#endif // FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
