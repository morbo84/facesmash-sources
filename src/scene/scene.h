#ifndef FACE_SMASH_SCENE_SCENE_H
#define FACE_SMASH_SCENE_SCENE_H


#include "../types/types.hpp"


namespace gamee {


class GameRenderer;


struct Scene {
    virtual void update(GameRenderer &, delta_type) = 0;
    virtual void entering() = 0;
    virtual void leaving() = 0;
};


}


#endif // FACE_SMASH_SCENE_SCENE_H
