#ifndef FACE_SMASH_SYSTEM_AV_RECORDER_SYSTEM_H
#define FACE_SMASH_SYSTEM_AV_RECORDER_SYSTEM_H


#include <memory>
#include <functional>
#include "../common/types.h"


namespace gamee {


struct AvRecorderEvent;
struct GameRenderer;


class AvRecorderSystem final {
    static constexpr auto bucket = 3;

public:
    AvRecorderSystem();
    ~AvRecorderSystem();

    void init();

    void receive(const AvRecorderEvent &) noexcept;

    void update(GameRenderer &, delta_type, std::function<void(void)>);

private:
    std::unique_ptr<unsigned char[]> pixels;
    delta_type accumulator;
    bool firstFrame;
    int pitch;
    int remaining;
};


}


#endif // FACE_SMASH_SYSTEM_AV_RECORDER_SYSTEM_H
