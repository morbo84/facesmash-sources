#ifndef FACE_SMASH_TIME_CLOCK_H
#define FACE_SMASH_TIME_CLOCK_H


#include "../common/types.h"


namespace gamee {


struct ClockEvent;


struct Clock final {
    Clock();

    void pause() noexcept;
    void unpause() noexcept;

    delta_type ticks() noexcept;
    delta_type time() const noexcept;

    void delay(delta_type) const noexcept;

private:
    delta_type previous;
    delta_type gap;
    bool paused;
};


}


#endif // FACE_SMASH_TIME_CLOCK_H
