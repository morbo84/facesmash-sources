#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../settings/settings.h"
#include "scene_menu.h"


namespace gamee {


void SceneMenu::update(GameRenderer &renderer, delta_type) {
    uiButtonSystem.update(registry);
    renderingSystem.update(registry, renderer);
}


void SceneMenu::entering() {
    auto &textureCache = Locator::TextureCache::ref();
    Settings settings;

    auto camera = registry.create();
    registry.assign<Transform>(camera, 0.f, 0.f);
    registry.attach<Camera>(camera);

    auto button = registry.create();
    registry.assign<Renderable>(button);
    registry.assign<Transform>(button, settings.logicalWidth() / 2.f - 80, settings.logicalHeight() / 2.f - 80);
    registry.assign<Sprite>(button, textureCache.handle("ui/buttons"), 160_ui16, 160_ui16, 160_ui16, 160_ui16, 0_ui16, 160_ui16);
    registry.assign<BoundingBox>(button, 160, 160);
    registry.assign<UIButton>(button, UIButton::Action::PLAY);
}


void SceneMenu::leaving() {
    registry.reset();
}


}
