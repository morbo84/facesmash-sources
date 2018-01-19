#ifndef FACE_SMASH_SYSTEM_HUD_SYSTEM_H
#define FACE_SMASH_SYSTEM_HUD_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


struct HudSystem final {
    void update(Registry &, GameRenderer &);
};


}


#endif // FACE_SMASH_SYSTEM_HUD_SYSTEM_H
