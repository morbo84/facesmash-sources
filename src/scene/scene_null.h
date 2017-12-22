#ifndef FACE_SMASH_SCENE_NULL_H
#define FACE_SMASH_SCENE_NULL_H


#include "scene.h"


namespace gamee {


struct SceneNull final: Scene {
    void update(GameRenderer &, delta_type) override;
    void entering() override;
    void leaving() override;
};


}


#endif // FACE_SMASH_SCENE_NULL_H
