#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "scene_game.h"


namespace gamee {


#ifdef DEBUG
void SceneGame::addDebugStuff() {
    registry.attach<TimeDebug>(registry.create<Renderable>(), 0.f);
    registry.attach<FPSDebug>(registry.create<Renderable>());
}


void SceneGame::addSmashButtons() {
    auto addButton = [](Registry &registry, SmashType type, TextureCache::resource_type emoji, float x, float y) {
        auto &textureCache = Locator::TextureCache::ref();
        auto button = registry.create();
        auto handle = textureCache.handle(emoji);

        registry.assign<SmashButton>(button, type);
        registry.assign<Transform>(button, x, y);
        registry.assign<Sprite>(button, handle, handle->width(), handle->height(), 96, 96);
        registry.assign<Renderable>(button);
        registry.assign<BoundingBox>(button, 96, 96);
    };

    const auto x = logicalWidth - 96 - 16;

    auto y = (logicalHeight - 96 * 7 - 32 * 6) / 2;
    addButton(registry, SmashType::ANGRY, "emoji/angry", x, y);
    y += 96 + 32;
    addButton(registry, SmashType::DISGUSTED, "emoji/disgusted", x, y);
    y += 96 + 32;
    addButton(registry, SmashType::HAPPY, "emoji/happy", x, y);
    y += 96 + 32;
    addButton(registry, SmashType::RESTED, "emoji/rested", x, y);
    y += 96 + 32;
    addButton(registry, SmashType::SURPRISED, "emoji/surprised", x, y);
    y += 96 + 32;
    addButton(registry, SmashType::FEARFUL, "emoji/fearful", x, y);
    y += 96 + 32;
    addButton(registry, SmashType::SAD, "emoji/sad", x, y);
}
#endif // DEBUG


void SceneGame::addHUDScore() {
    auto &textureCache = Locator::TextureCache::ref();

    auto label = registry.create<Renderable>();
    auto handle = textureCache.handle("hud/score");
    registry.assign<HUD>(label, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(label, 32.f, 32.f);

    auto score = registry.create<Renderable>();
    registry.attach<HUDScore>(score);
    registry.assign<Transform>(score, 32.f + handle->width() + 16.f, 32.f);
}


void SceneGame::update(GameRenderer &renderer, delta_type delta) {
    // sum what remains from the previous step
    accumulator += delta;

#ifdef DEBUG
    smashButtonSystem.update(registry);
#endif // DEBUG

    destroyLaterSystem.update(registry, delta);
    faceSmashSystem.update(registry);
    faceSpawnerSystem.update(registry, delta);

    // invoke systems at 50 fps (but for rendering and few other systems)
    while(accumulator >= msPerUpdate) {
        movementSystem.update(registry, msPerUpdate);
        // consume a token
        accumulator -= msPerUpdate;
    }

    fadeAnimationSystem.update(registry, delta);
    rotationAnimationSystem.update(registry, delta);
    spriteAnimationSystem.update(registry, delta);

    scoreSystem.update(registry, renderer);

    renderingSystem.update(registry, renderer);
    hudSystem.update(registry, renderer, delta);
}


void SceneGame::entering() {
    auto camera = registry.create();
    registry.attach<Camera>(camera);
    registry.assign<Transform>(camera, 0.f, 0.f);

#if DEBUG
    addDebugStuff();
    addSmashButtons();
#endif // DEBUG

    addHUDScore();
}


void SceneGame::leaving() {
    registry.reset();
}


}
