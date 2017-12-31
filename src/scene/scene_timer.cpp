#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "scene_timer.h"


namespace gamee {


#ifdef DEBUG
void SceneTimer::addDebugStuff() {
    registry.attach<TimeDebug>(registry.create<Renderable>(), 0.f);
    registry.attach<FPSDebug>(registry.create<Renderable>());
}


void SceneTimer::addSmashButtons() {
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


void SceneTimer::addBackgroundFrame() {
    if(Locator::Camera::ref().available()) {
        auto handle = Locator::TextureCache::ref().handle("visage/frame");
        auto frame = registry.create();

        registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<Renderable>(frame, 0.f, 20);
        registry.assign<Transform>(frame, (logicalWidth - handle->width()) / 2.f, (logicalHeight - handle->height()) / 2.f);
    }
}


void SceneTimer::addHUDScore() {
    auto &textureCache = Locator::TextureCache::ref();

    auto label = registry.create<Renderable>();
    auto handle = textureCache.handle("hud/score");
    registry.assign<HUD>(label, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(label, 32.f, 32.f);

    auto score = registry.create<Renderable>();
    registry.attach<HUDScore>(score);
    registry.assign<Transform>(score, 32.f + handle->width() + 16.f, 32.f);
}


void SceneTimer::update(GameRenderer &renderer, delta_type delta) {
    // sum what remains from the previous step
    accumulator += delta;

#ifdef DEBUG
    smashButtonSystem.update(registry);
#endif // DEBUG

    destroyLaterSystem.update(registry, delta);
    faceSmashSystem.update(registry);
    frameSystem.update();
    comboSystem.update(registry, delta);
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

    scoreSystem.update(registry, renderer, delta);
    timerSystem.update(registry, renderer, delta);

    renderingSystem.update(registry, renderer);
    hudSystem.update(registry, renderer, delta);
}


void SceneTimer::entering() {
    auto camera = registry.create();
    registry.attach<Camera>(camera);
    registry.assign<Transform>(camera, 0.f, 0.f);

#if DEBUG
    addDebugStuff();
    addSmashButtons();
#endif // DEBUG

    addBackgroundFrame();
    addHUDScore();
}


void SceneTimer::leaving() {
    registry.reset();
}


}
