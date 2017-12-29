#ifndef FACE_SMASH_SCENE_SCENE_H
#define FACE_SMASH_SCENE_SCENE_H


#include "../common/types.h"


namespace gamee {


class GameRenderer;


struct Scene {
    virtual ~Scene() = default;

    virtual void update(GameRenderer &, delta_type) = 0;
    virtual void entering() = 0;
    virtual void leaving() = 0;
};


}


#endif // FACE_SMASH_SCENE_SCENE_H
