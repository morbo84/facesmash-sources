#ifndef FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
#define FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H


#include <memory>
#include <functional>
#include "../common/types.h"


namespace gamee {


struct GameRenderer;


struct RecordingSystem final {
    void init(GameRenderer &);

    void update(GameRenderer &, delta_type, std::function<void(bool)>);

private:
    std::unique_ptr<unsigned char[]> pixels{nullptr};
    delta_type accumulator{0_ui32};
    bool firstFrame{true};
    bool ready{false};
    int pitch{0};
};


}


#endif // FACE_SMASH_SYSTEM_RECORDING_SYSTEM_H
