#ifndef FACE_SMASH_SYSTEM_TIMER_SYSTEM_H
#define FACE_SMASH_SYSTEM_TIMER_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


class TimerSystem final {
    static constexpr UInt16 duration = 30000;

public:
    void update(Registry &, GameRenderer &, delta_type);

private:
    UInt16 elapsed{0_ui16};
};


}


#endif // FACE_SMASH_SYSTEM_TIMER_SYSTEM_H
