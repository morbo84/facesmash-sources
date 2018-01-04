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

        if(cameraService.height() > cameraService.width()) {
            registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), logicalWidth, handle->height() * logicalWidth / handle->width());
            registry.assign<Transform>(frame, 0.f, (logicalHeight - handle->height()) / 2.f);
            registry.assign<Renderable>(frame, 0.f, 20);
        } else {
            const int width = handle->width() * logicalWidth / handle->height();
            const int height = logicalWidth;

            registry.assign<Sprite>(frame, handle, handle->width(), handle->height(), width, height);
            registry.assign<Transform>(frame, (height - width) / 2.f, (width - height) / 2.f);
            registry.assign<Renderable>(frame, -90.f, 20);
        }
    }
}


void SceneSystem::gameUI(Registry &registry) {
    auto handle = Locator::TextureCache::ref().handle("game/banner");

    auto top = registry.create();
    registry.assign<Sprite>(top, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(top, 0.f, 0.f);
    registry.assign<Renderable>(top, 0.f, 30);

    auto bottom = registry.create();
    registry.assign<Sprite>(bottom, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(bottom, 0.f, 1600.f);
    registry.assign<Renderable>(bottom, 0.f, 30);

    // TODO
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


void SceneSystem::timer(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();
    auto timer = registry.create<Renderable>();
    auto handle = textureCache.handle("hud/time");

    registry.assign<HUD>(timer, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(timer, logicalWidth - 2.f * handle->width(), 32.f);

    auto time = registry.create<Renderable>();
    registry.attach<GameTimer>(time, 60000_ui32);
    registry.assign<Transform>(time, logicalWidth - handle->width() + 16.f, 32.f);
}


void SceneSystem::camera(Registry &registry) {
    if(!registry.has<Camera>()) {
        auto camera = registry.create();
        registry.assign<Transform>(camera, 0.f, 0.f);
        registry.attach<Camera>(camera);
    }
}


void SceneSystem::spawner(Registry &registry) {
    registry.assign<SpawnRequest>(registry.create(), SDL_Rect{440, 100, 200, 300}, 140, 800, true, true, 1800_ui32, 1200_ui32, 1800_ui32);
    registry.assign<SpawnRequest>(registry.create(), SDL_Rect{490, logicalHeight - 100, 100, 50}, 140, 800, true, false, 2000_ui32, 0xFFFFFFFF_ui32, 4750_ui32);
    registry.assign<SpawnRequest>(registry.create(), SDL_Rect{100, logicalHeight - 100, 100, 50}, 200, 400, false, false, 400_ui32, 0xFFFFFFFF_ui32, 2100_ui32);
    registry.assign<SpawnRequest>(registry.create(), SDL_Rect{logicalWidth - 200, logicalHeight - 100, 100, 50}, 200, 400, false, true, 400_ui32, 0xFFFFFFFF_ui32, 2300_ui32);
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

    auto entity = registry.create();
    auto emoji = textureCache.handle("emoji/happy");

    auto bg = registry.create();
    auto bgSprite = textureCache.handle("menu/bg");
    registry.assign<Renderable>(bg, 0.f, 10);
    registry.assign<Sprite>(bg, bgSprite, bgSprite->width(), bgSprite->height(), bgSprite->width(), bgSprite->height());
    registry.assign<Transform>(bg, 0.f, 0.f);

    registry.assign<Renderable>(entity);
    registry.assign<Transform>(entity, logicalWidth / 2.f - emoji->width() / 2, logicalHeight / 2.f + emoji->height() * 2);
    registry.assign<Sprite>(entity, emoji, emoji->width(), emoji->height(), emoji->width(), emoji->height());
    registry.assign<BoundingBox>(entity, emoji->width(), emoji->height());
    registry.assign<UIButton>(entity, UIButton::Action::PLAY);
}


void SceneSystem::theGame(Registry &registry) {
    backgroundFrame(registry);
    gameUI(registry);
    score(registry);
    timer(registry);
    spawner(registry);

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
            case SceneType::THE_GAME:
                theGame(registry);
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
