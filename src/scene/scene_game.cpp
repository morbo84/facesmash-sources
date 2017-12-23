#include "scene_game.h"


namespace gamee {


void SceneGame::update(GameRenderer &renderer, delta_type delta) {
    // sum what remains from the previous step
    accumulator += delta;

    // invoke systems at 50 fps (but for rendering and few other systems)
    while(accumulator >= msPerUpdate) {
        pathUpdaterSystem.update(registry, msPerUpdate);
        // consume a token
        accumulator -= msPerUpdate;
    }

    fadeAnimationSystem.update(registry, delta);
    rotationAnimationSystem.update(registry, delta);
    spriteAnimationSystem.update(registry, delta);
    faceSpawnerSystem.update(registry, delta);

    renderingSystem.update(registry, renderer);
    hudSystem.update(registry, renderer, delta);
}


void SceneGame::entering() {
    // TODO
}


void SceneGame::leaving() {
    registry.reset();
}


}
