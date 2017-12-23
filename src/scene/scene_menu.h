#ifndef FACE_SMASH_SCENE_MENU_H
#define FACE_SMASH_SCENE_MENU_H


#include "../system/ui_button_system.h"
#include "../system/rendering_system.h"
#include "../types/types.hpp"
#include "scene.h"


namespace gamee {


struct SceneMenu final: Scene {
    void update(GameRenderer &, delta_type) override;
    void entering() override;
    void leaving() override;

private:
    Registry registry;
    UIButtonSystem uiButtonSystem;
    RenderingSystem renderingSystem;
};


}


#endif // FACE_SMASH_SCENE_MENU_H
