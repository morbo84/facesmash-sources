#ifndef FACE_SMASH_SCENE_MENU_H
#define FACE_SMASH_SCENE_MENU_H


#include "scene.h"


namespace gamee {


struct SceneMenu final: Scene {
    void update(GameRenderer &, delta_type) override;
    void entering() override;
    void leaving() override;
};


}


#endif // FACE_SMASH_SCENE_MENU_H
