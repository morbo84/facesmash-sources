#include <cassert>
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "common.h"
#include "ui_factory.h"


namespace gamee {


entity_type createPanel(Registry &registry, PanelType type, float x, float y, int w, int h) {
    auto entity = registry.create();
    registry.assign<Panel>(entity, w, h, type);
    registry.assign<Transform>(entity, entity, x, y);
    return entity;
}


entity_type createUIButton(Registry &registry, entity_type parent, UIAction action, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("ui/buttons");
    auto entity = createSprite(registry, parent, handle, z);

    const auto width = handle->width() / 12;
    const auto height = handle->height() / 14;

    switch(action) {
    case UIAction::ACHIEVEMENTS:
        setSpriteGeometry(registry, entity, 1024, 1280, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::CREDITS:
        setSpriteGeometry(registry, entity, 1024, 1024, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::EXIT:
        setSpriteGeometry(registry, entity, 512, 384, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::MENU:
        setSpriteGeometry(registry, entity, 512, 1536, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::MENU_CLOSE_UP:
        setSpriteGeometry(registry, entity, 0, 512, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::MENU_CLOSE_DOWN:
        setSpriteGeometry(registry, entity, 1024, 768, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::RELOAD:
        setSpriteGeometry(registry, entity, 0, 1536, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::SAVE:
        setSpriteGeometry(registry, entity, 0, 1280, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::SETTINGS:
        setSpriteGeometry(registry, entity, 0, 1152, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::SUPPORT:
        setSpriteGeometry(registry, entity, 0, 896, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::SWITCH_AUDIO:
        setSpriteGeometry(registry, entity, 0, 768, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::SWITCH_VIDEO:
        setSpriteGeometry(registry, entity, 0, 1280, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::THE_GAME:
        setSpriteGeometry(registry, entity, 1024, 128, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::TRAINING:
        setSpriteGeometry(registry, entity, 0, 256, width, height, 4_ui8, 1_ui8);
        break;
    case UIAction::EASTER_EGG:
    default:
        assert(false);
        break;
    };

    setSpriteSize(registry, entity, width, height);
    registry.assign<BoundingBox>(entity, width, height);
    registry.assign<UIButton>(entity, entity, action);

    return entity;
}


entity_type createPopupUIButton(Registry &registry, entity_type parent, UIAction action, int z) {
    auto entity = createUIButton(registry, parent, action, z);
    const auto &sprite = registry.get<Sprite>(entity);
    auto &button = registry.get<UIButton>(entity);

    button.popup = true;
    button.w = sprite.w;
    button.h = sprite.h;

    return entity;
}


static void internalCreateHUD(Registry &registry, entity_type entity, entity_type parent, SDLTextureHandle handle, int z) {
    registry.assign<Renderable>(entity, 0.f, z);
    registry.assign<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, parent, 0.f, 0.f);
}


entity_type createHUD(Registry &registry, entity_type parent, SDLTextureHandle handle, int z) {
    const auto entity = registry.create();
    internalCreateHUD(registry, entity, parent, handle, z);
    return entity;
}


entity_type createHUD(Registry &registry, SDLTextureHandle handle, int z) {
    const auto entity = registry.create();
    internalCreateHUD(registry, entity, entity, handle, z);
    return entity;
}


}
