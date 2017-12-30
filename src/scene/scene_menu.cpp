#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "scene_menu.h"


namespace gamee {


void SceneMenu::update(GameRenderer &renderer, delta_type) {
    uiButtonSystem.update(registry);
    renderingSystem.update(registry, renderer);
}


void SceneMenu::entering() {
    auto &textureCache = Locator::TextureCache::ref();

    auto camera = registry.create();
    registry.assign<Transform>(camera, 0.f, 0.f);
    registry.attach<Camera>(camera);

    auto button = registry.create();
    auto handle = textureCache.handle("emoji/happy");

    registry.assign<Renderable>(button);
    registry.assign<Transform>(button, logicalWidth / 2.f - 64, logicalHeight / 2.f - 64);
    registry.assign<Sprite>(button, handle, handle->width(), handle->height(), 128, 128);
    registry.assign<BoundingBox>(button, 128, 128);
    registry.assign<UIButton>(button, UIButton::Action::CHALLENGE);
}


void SceneMenu::leaving() {
    registry.reset();
}


}
