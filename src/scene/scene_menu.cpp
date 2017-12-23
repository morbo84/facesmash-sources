#include "scene_menu.h"


namespace gamee {


void SceneMenu::update(GameRenderer &renderer, delta_type) {
    uiButtonSystem.update(registry);
    renderingSystem.update(registry, renderer);
}


void SceneMenu::entering() {
    // TODO
}


void SceneMenu::leaving() {
    registry.reset();
}


}
