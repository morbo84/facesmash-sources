#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../settings/settings.h"
#include "scene_game.h"


namespace gamee {


#ifdef DEBUG
void SceneGame::addDebugStuff() {
    auto fps = registry.create();
    auto time = registry.create();
    auto debug = registry.create();

    registry.assign<Renderable>(fps);
    registry.assign<Renderable>(time);
    registry.assign<Debug>(debug, fps, time, 0.f);
}


void SceneGame::addSmashButtons() {
    Settings settings;

    auto addButton = [](Registry &registry, SmashType type, TextureCache::resource_type emoji, float x, float y) {
        auto &textureCache = Locator::TextureCache::ref();
        auto button = registry.create();

        registry.assign<SmashButton>(button, type);
        registry.assign<Transform>(button, x, y);
        registry.assign<Sprite>(button, textureCache.handle(emoji), 128_ui16, 128_ui16, 64_ui16, 64_ui16);
        registry.assign<Renderable>(button);
        registry.assign<BoundingBox>(button, 64, 64);
    };

    const auto x = settings.logicalWidth() - 64 - 16;

    auto y = (settings.logicalHeight() - 64 * 7 - 32 * 6) / 2;
    addButton(registry, SmashType::ANGRY, "emoji/angry", x, y);
    y += 64 + 32;
    addButton(registry, SmashType::DISGUSTED, "emoji/disgusted", x, y);
    y += 64 + 32;
    addButton(registry, SmashType::HAPPY, "emoji/happy", x, y);
    y += 64 + 32;
    addButton(registry, SmashType::RESTED, "emoji/rested", x, y);
    y += 64 + 32;
    addButton(registry, SmashType::SURPRISED, "emoji/surprised", x, y);
    y += 64 + 32;
    addButton(registry, SmashType::FEARFUL, "emoji/fearful", x, y);
    y += 64 + 32;
    addButton(registry, SmashType::SAD, "emoji/sad", x, y);
}
#endif // DEBUG


void SceneGame::update(GameRenderer &renderer, delta_type delta) {
    // sum what remains from the previous step
    accumulator += delta;

#ifdef DEBUG
    smashButtonSystem.update(registry);
#endif // DEBUG

    faceSmashSystem.update(registry, delta);

    // invoke systems at 50 fps (but for rendering and few other systems)
    while(accumulator >= msPerUpdate) {
        movementSystem.update(registry, msPerUpdate);
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
    auto camera = registry.create();
    registry.assign<Transform>(camera, 0.f, 0.f);
    registry.attach<Camera>(camera);

#if DEBUG
    addDebugStuff();
    addSmashButtons();
#endif // DEBUG
}


void SceneGame::leaving() {
    registry.reset();
}


}
