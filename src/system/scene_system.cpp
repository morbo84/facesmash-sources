#include <algorithm>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "scene_system.h"


namespace gamee {


void SceneSystem::backgroundFrame(Registry &registry) {
    auto handle = Locator::TextureCache::ref().handle("camera/frame");

    if(handle) {
        auto &cameraService = Locator::Camera::ref();
        auto frame = registry.create();

        registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<Renderable>(frame, cameraService.height() > cameraService.width() ? 0.f : -90.f, 20);
        registry.assign<Transform>(frame, (logicalWidth - handle->width()) / 2.f, (logicalHeight - handle->height()) / 2.f);
    }
}


void SceneSystem::score(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    auto label = registry.create<Renderable>();
    auto handle = textureCache.handle("hud/score");
    registry.assign<HUD>(label, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(label, 32.f, 32.f);

    auto score = registry.create<Renderable>();
    registry.attach<PlayerScore>(score);
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
    auto addButton = [](Registry &registry, FaceType type, TextureCache::resource_type emoji, float x, float y) {
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
    addButton(registry, FaceType::ANGRY, "emoji/angry", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::DISGUSTED, "emoji/disgusted", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::HAPPY, "emoji/happy", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::RESTED, "emoji/rested", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::SURPRISED, "emoji/surprised", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::FEARFUL, "emoji/fearful", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::SAD, "emoji/sad", x, y);
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

    auto training = registry.create();
    auto happy = textureCache.handle("emoji/happy");

    registry.assign<Renderable>(training);
    registry.assign<Transform>(training, 116.f, logicalHeight / 2.f - happy->height() / 2);
    registry.assign<Sprite>(training, happy, happy->width(), happy->height(), happy->width(), happy->height());
    registry.assign<BoundingBox>(training, happy->width(), happy->height());
    registry.assign<UIButton>(training, UIButton::Action::TRAINING);

    auto timer = registry.create();
    auto sad = textureCache.handle("emoji/sad");

    registry.assign<Renderable>(timer);
    registry.assign<Transform>(timer, 476.f, logicalHeight / 2.f - sad->height() / 2);
    registry.assign<Sprite>(timer, sad, sad->width(), sad->height(), sad->width(), sad->height());
    registry.assign<BoundingBox>(timer, sad->width(), sad->height());
    registry.assign<UIButton>(timer, UIButton::Action::TIMER);

    auto challenge = registry.create();
    auto surprised = textureCache.handle("emoji/surprised");

    registry.assign<Renderable>(challenge);
    registry.assign<Transform>(challenge, 836.f, logicalHeight / 2.f - surprised->height() / 2);
    registry.assign<Sprite>(challenge, surprised, surprised->width(), surprised->height(), surprised->width(), surprised->height());
    registry.assign<BoundingBox>(challenge, surprised->width(), surprised->height());
    registry.assign<UIButton>(challenge, UIButton::Action::CHALLENGE);
}


void SceneSystem::training(Registry &registry) {
    backgroundFrame(registry);
    score(registry);

    // default spawner
    registry.assign<SpawnRequest>(registry.create(), SDL_Rect{440, 100, 200, 300}, -140, 1200_ui32, 1800_ui32);

    // TODO add stuff to lifes management

#if DEBUG
    smashButtons(registry);
#endif // DEBUG
}


void SceneSystem::timer(Registry &registry) {
    backgroundFrame(registry);
    score(registry);

    // default spawner
    registry.assign<SpawnRequest>(registry.create(), SDL_Rect{440, 100, 200, 300}, -140, 1200_ui32, 1800_ui32);

    auto &textureCache = Locator::TextureCache::ref();

    // timer & time

    auto timer = registry.create<Renderable>();
    auto handle = textureCache.handle("hud/time");
    registry.assign<HUD>(timer, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(timer, logicalWidth - 2.f * handle->width(), 32.f);

    auto time = registry.create<Renderable>();
    registry.attach<GameTimer>(time, 30000_ui32);
    registry.assign<Transform>(time, logicalWidth - handle->width() + 16.f, 32.f);

#if DEBUG
    smashButtons(registry);
#endif // DEBUG
}


void SceneSystem::challenge(Registry &registry) {
    backgroundFrame(registry);

    // default spawner
    registry.assign<SpawnRequest>(registry.create(), SDL_Rect{440, 100, 200, 300}, -140, 1200_ui32, 1800_ui32);

    auto handle = Locator::TextureCache::ref().handle("icon/end");
    auto end = registry.create<Renderable>();
    registry.assign<Transform>(end, logicalWidth / 2.f - handle->width() / 2.f, handle->height() + 16.f);
    registry.assign<Sprite>(end, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<BoundingBox>(end, handle->width(), handle->height());
    registry.assign<UIButton>(end, UIButton::Action::MENU);

#if DEBUG
    smashButtons(registry);
#endif // DEBUG
}


void SceneSystem::gameOver(Registry &registry) {
    // TODO
}


void SceneSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<SceneChangeRequest>()) {
        auto &request = registry.get<SceneChangeRequest>();

        request.remaining -= std::min(request.remaining, delta);

        if(0 == request.remaining) {
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
            case SceneType::GAME_OVER:
                gameOver(registry);
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
