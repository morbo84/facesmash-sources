#ifndef FACE_SMASH_SYSTEM_RENDERING_SYSTEM_H
#define FACE_SMASH_SYSTEM_RENDERING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


struct RenderingSystem final {
    void update(Registry &, GameRenderer &);
};


}


#endif // FACE_SMASH_SYSTEM_RENDERING_SYSTEM_H
