#ifndef FACE_SMASH_SYSTEM_DEBUG_SYSTEM_H
#define FACE_SMASH_SYSTEM_DEBUG_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct DebugEvent;


struct DebugSystem final {
    DebugSystem();
    ~DebugSystem();

    void receive(const DebugEvent &) noexcept;

    void update(Registry &, delta_type);

private:
    delta_type last;
};


}


#endif // FACE_SMASH_SYSTEM_DEBUG_SYSTEM_H
