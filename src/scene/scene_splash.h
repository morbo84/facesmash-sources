#ifndef FACE_SMASH_SCENE_SPLASH_H
#define FACE_SMASH_SCENE_SPLASH_H


#include "scene.h"


namespace gamee {


struct SceneSplash final: Scene {
    void update(GameRenderer &, delta_type) override;
    void entering() override;
    void leaving() override;
};


}


#endif // FACE_SMASH_SCENE_SPLASH_H
