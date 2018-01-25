#ifndef FACE_SMASH_SYSTEM_RENDERING_SYSTEM_H
#define FACE_SMASH_SYSTEM_RENDERING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


class RenderingSystem final {
    void game(Registry &, GameRenderer &);
    void hud(Registry &, GameRenderer &);
    void debug(Registry &, GameRenderer &);

public:
    void update(Registry &, GameRenderer &);
};


}


#endif // FACE_SMASH_SYSTEM_RENDERING_SYSTEM_H
