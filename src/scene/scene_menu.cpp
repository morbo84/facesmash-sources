#include "../component/component.hpp"
#include "../settings/settings.h"
#include "scene_menu.h"


namespace gamee {


void SceneMenu::update(GameRenderer &renderer, delta_type) {
    uiButtonSystem.update(registry);
    renderingSystem.update(registry, renderer);
}


void SceneMenu::entering() {
    Settings settings;

    auto camera = registry.create();
    registry.assign<Transform>(camera, settings.logicalWidth() / 2.f, settings.logicalHeight() / 2.f, 128.f);
    registry.attach<Camera>(camera, settings.logicalWidth() / 2.f, settings.logicalHeight() / 2.f);

    // TODO create ui
}


void SceneMenu::leaving() {
    registry.reset();
}


}
