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
    auto &textureCache = Locator::TextureCache::ref();

    auto top = registry.create();
    auto bottom = registry.create();
    auto bannerHandle = textureCache.handle("game/banner");

    registry.assign<Sprite>(top, bannerHandle, bannerHandle->width(), bannerHandle->height(), bannerHandle->width(), bannerHandle->height());
    registry.assign<Transform>(top, 0.f, 0.f);
    registry.assign<Renderable>(top, 0.f, 30);

    registry.assign<Sprite>(bottom, bannerHandle, bannerHandle->width(), bannerHandle->height(), bannerHandle->width(), bannerHandle->height());
    registry.assign<Transform>(bottom, 0.f, 1600.f);
    registry.assign<Renderable>(bottom, 0.f, 30);

    auto scoreLabel = registry.create<Renderable>();
    auto scoreHandle = textureCache.handle("hud/score");
    registry.assign<HUD>(scoreLabel, scoreHandle, scoreHandle->width(), scoreHandle->height(), scoreHandle->width(), scoreHandle->height());
    registry.assign<Transform>(scoreLabel, 32.f, 32.f);

    auto score = registry.create<Renderable>();
    registry.attach<PlayerScore>(score);
    registry.assign<Transform>(score, 32.f + scoreHandle->width() + 16.f, 32.f);

    auto timer = registry.create<Renderable>();
    auto timerHandle = textureCache.handle("hud/time");
    registry.assign<HUD>(timer, timerHandle, timerHandle->width(), timerHandle->height(), timerHandle->width(), timerHandle->height());
    registry.assign<Transform>(timer, logicalWidth - 2.f * timerHandle->width(), 32.f);

    auto time = registry.create<Renderable>();
    registry.attach<GameTimer>(time, 60999_ui32);
    registry.assign<Transform>(time, logicalWidth - timerHandle->width() + 16.f, 32.f);

    auto bonus = registry.create();
    registry.assign<Renderable>(bonus, 0.f, 200);
    registry.assign<Transform>(bonus, logicalWidth / 2.f, bannerHandle->height() - 128.f);
    registry.attach<BonusSmash>(bonus);
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

    auto y = (logicalHeight - 96 * numberOfFaces - 32 * (numberOfFaces - 1)) / 2;
    addButton(registry, FaceType::ANGRY, "emoji/angry", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::DISGUSTED, "emoji/disgusted", x, y);
    y += 96 + 32;
    addButton(registry, FaceType::HAPPY, "emoji/happy", x, y);
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
    registry.reset();

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
    registry.reset();

    auto &textureCache = Locator::TextureCache::ref();

    auto entity = registry.create();
    auto emoji = textureCache.handle("emoji/happy");

    auto bg = registry.create();
    auto bgSprite = textureCache.handle("menu/bg");
    registry.assign<Renderable>(bg, 0.f, 10);
    registry.assign<Sprite>(bg, bgSprite, bgSprite->width(), bgSprite->height(), bgSprite->width(), bgSprite->height());
    registry.assign<FadeAnimation>(bg, 0, 255, 600_ui32);
    registry.assign<Transform>(bg, 0.f, 0.f);

    registry.assign<Renderable>(entity);
    registry.assign<Transform>(entity, logicalWidth / 2.f - emoji->width() / 2, logicalHeight / 2.f - emoji->height() / 2);
    registry.assign<Sprite>(entity, emoji, emoji->width(), emoji->height(), emoji->width(), emoji->height());
    registry.assign<BoundingBox>(entity, emoji->width(), emoji->height());
    registry.assign<UIButton>(entity, UIAction::PLAY);
}


void SceneSystem::theGame(Registry &registry) {
    registry.reset();

    backgroundFrame(registry);
    gameUI(registry);

#if DEBUG
    smashButtons(registry);
#endif // DEBUG

    registry.attach<LetsPlay>(registry.create(), SDL_Rect{logicalWidth / 8, logicalHeight / 8, 6 * logicalWidth / 8, 6 * logicalHeight / 8});
}


void SceneSystem::gameOver(Registry &registry) {
    auto &textureCache = Locator::TextureCache::ref();

    // stop playing
    registry.destroy(registry.attachee<LetsPlay>());

    auto reload = registry.create();
    auto reloadHandle = textureCache.handle("end/reload");
    registry.assign<Renderable>(reload, 0.f, 250);
    registry.assign<Sprite>(reload, reloadHandle, reloadHandle->width(), reloadHandle->height(), reloadHandle->width(), reloadHandle->height());
    registry.assign<Transform>(reload, logicalWidth / 2.f - reloadHandle->width() / 2.f, logicalHeight / 2.f - reloadHandle->height() / 2.f);
    registry.assign<BoundingBox>(reload, reloadHandle->width(), reloadHandle->height());
    registry.assign<UIButton>(reload, UIAction::PLAY);
}


void SceneSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<SceneChangeRequest>()) {
        auto &request = registry.get<SceneChangeRequest>();

        request.remaining -= std::min(request.remaining, delta);

        if(0 == request.remaining) {
            // copy on purpose, we are going to destroy the entity after all
            auto scene = request.scene;

            // remove the scene change request before to serve it
            registry.destroy(registry.attachee<SceneChangeRequest>());

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
