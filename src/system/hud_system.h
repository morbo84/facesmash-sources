#ifndef FACE_SMASH_SYSTEM_HUD_SYSTEM_H
#define FACE_SMASH_SYSTEM_HUD_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


class HudSystem final {
#if DEBUG
    void debug(Registry &, delta_type);
#endif // DEBUG

    void update(Registry &, GameRenderer &);

public:
    void update(Registry &, GameRenderer &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_HUD_SYSTEM_H
