#ifndef FACE_SMASH_SCENE_TRAINING_H
#define FACE_SMASH_SCENE_TRAINING_H


#include "../common/types.h"
#include "scene.h"


namespace gamee {


struct SceneTraining final: public Scene {
    void update(GameRenderer &, delta_type) override;
    void entering() override;
    void leaving() override;
};


}


#endif // FACE_SMASH_SCENE_TRAINING_H
