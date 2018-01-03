#ifndef FACE_SMASH_SYSTEM_TIMER_SYSTEM_H
#define FACE_SMASH_SYSTEM_TIMER_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


struct TimerSystem final {
    void update(Registry &, GameRenderer &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_TIMER_SYSTEM_H
