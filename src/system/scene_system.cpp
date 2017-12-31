#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "scene_system.h"


namespace gamee {


void SceneSystem::splashScreen(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto rainbow = textureCache.handle("logo/rainbow");

    auto orange = registry.create<Renderable>();
    registry.assign<Sprite>(orange, rainbow, 150, 33, 80, 20, 0, 0);
    registry.assign<Transform>(orange, logicalWidth / 2.f - 200, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(orange, 0, 255, 400_ui32);

    auto yellow = registry.create<Renderable>();
    registry.assign<Sprite>(yellow, rainbow, 150, 33, 80, 20, 150, 0);
    registry.assign<Transform>(yellow, logicalWidth / 2.f - 120, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(yellow, 0, 255, 800_ui32);

    auto green = registry.create<Renderable>();
    registry.assign<Sprite>(green, rainbow, 150, 33, 80, 20, 300, 0);
    registry.assign<Transform>(green, logicalWidth / 2.f - 40, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(green, 0, 255, 1200_ui32);

    auto blue = registry.create<Renderable>();
    registry.assign<Sprite>(blue, rainbow, 150, 33, 80, 20, 450, 0);
    registry.assign<Transform>(blue, logicalWidth / 2.f + 40, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(blue, 0, 255, 1600_ui32);

    auto purple = registry.create<Renderable>();
    registry.assign<Sprite>(purple, rainbow, 150, 33, 80, 20, 600, 0);
    registry.assign<Transform>(purple, logicalWidth / 2.f + 120, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(purple, 0, 255, 2000_ui32);

    auto gamee = Locator::TextureCache::ref().handle("logo/gamee");

    auto company = registry.create<Renderable>();
    registry.assign<Sprite>(company, gamee, gamee->width(), gamee->height(), gamee->width(), gamee->height());
    registry.assign<Transform>(company, logicalWidth / 2.f - gamee->width() / 2.f, logicalHeight / 2.f - 60);
    registry.assign<FadeAnimation>(company, 0, 255, 2000_ui32);

    auto division = Locator::TextureCache::ref().handle("logo/division");

    auto dep = registry.create<Renderable>();
    registry.assign<Sprite>(dep, division, division->width(), division->height(), division->width(), division->height());
    registry.assign<Transform>(dep, logicalWidth / 2.f - division->width() / 2.f, logicalHeight / 2.f + 60);
    registry.assign<FadeAnimation>(dep, 0, 255, 2000_ui32);

    registry.attach<SceneChangeRequest>(registry.create(), SceneType::MENU_PAGE, 3000_ui32);
}


void SceneSystem::mainMenu(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto button = registry.create();
    auto happy = textureCache.handle("emoji/happy");

    registry.assign<Renderable>(button);
    registry.assign<Transform>(button, logicalWidth / 2.f - 64, logicalHeight / 2.f - 64);
    registry.assign<Sprite>(button, happy, happy->width(), happy->height(), 128, 128);
    registry.assign<BoundingBox>(button, 128, 128);
    registry.assign<UIButton>(button, UIButton::Action::TIMER);
}


void SceneSystem::training(Registry &registry) {
    // TODO
}


void SceneSystem::timer(Registry &registry) {
    backgroundFrame(registry);
    hudScore(registry);

#if DEBUG
    smashButtons(registry);
#endif // DEBUG
}


void SceneSystem::challenge(Registry &registry) {
    // TODO
}


void SceneSystem::backgroundFrame(Registry &registry) {
    if(Locator::Camera::ref().available()) {
        auto handle = Locator::TextureCache::ref().handle("visage/frame");
        auto frame = registry.create();

        registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<Renderable>(frame, 0.f, 20);
        registry.assign<Transform>(frame, (logicalWidth - handle->width()) / 2.f, (logicalHeight - handle->height()) / 2.f);
    }
}


void SceneSystem::hudScore(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto label = registry.create<Renderable>();
    auto handle = textureCache.handle("hud/score");
    registry.assign<HUD>(label, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(label, 32.f, 32.f);

    auto score = registry.create<Renderable>();
    registry.attach<HUDScore>(score);
    registry.assign<Transform>(score, 32.f + handle->width() + 16.f, 32.f);
}


void SceneSystem::camera(Registry &registry) {
    if(!registry.has<Camera>()) {
        auto camera = registry.create();
        registry.assign<Transform>(camera, 0.f, 0.f);
        registry.attach<Camera>(camera);
    }
}


#if DEBUG
void SceneSystem::smashButtons(Registry &registry) {
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


void SceneSystem::debug(Registry &registry) {
    if(!registry.has<TimeDebug>()) {
        registry.attach<TimeDebug>(registry.create<Renderable>(), 0.f);
    }

    if(!registry.has<FPSDebug>()) {
        registry.attach<FPSDebug>(registry.create<Renderable>());
    }
}
#endif // DEBUG


void SceneSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<SceneChangeRequest>()) {
        auto &request = registry.get<SceneChangeRequest>();

        request.elapsed += delta;

        if(!(request.elapsed < request.delay)) {
            // copy on purpose, we are going to reset the registry after all
            auto scene = request.scene;

            registry.reset();

            switch(scene) {
            case SceneType::SPLASH_SCREEN:
                splashScreen(registry);
                break;
            case SceneType::MENU_PAGE:
                mainMenu(registry);
                break;
            case SceneType::GAME_TRAINING:
                training(registry);
                break;
            case SceneType::GAME_TIMER:
                timer(registry);
                break;
            case SceneType::GAME_CHALLENGE:
                challenge(registry);
                break;
            }

            camera(registry);

#if DEBUG
            debug(registry);
#endif // DEBUG
        }
    }
}


}
