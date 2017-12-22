#include "scene_null.h"


namespace gamee {


void SceneNull::update(GameRenderer &, delta_type) {
    // a null scene has nothing to render, it's just a null object
}


void SceneNull::entering() {
    // a null scene has nothing to do when entering
}


void SceneNull::leaving() {
    // a null scene has nothing to do when leaving
}


}
