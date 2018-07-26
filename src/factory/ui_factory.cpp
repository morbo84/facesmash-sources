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

    const auto width = handle->width() / 5;
    const auto height = handle->height() / 6;

    switch(action) {
    case UIAction::ACHIEVEMENTS:
        setSpriteGeometry(registry, entity, 0, 0, width, height, 1_ui8);
        break;
    case UIAction::LEADERBOARD:
        setSpriteGeometry(registry, entity, 384, 384, width, height, 1_ui8);
        break;
    case UIAction::CREDITS:
    case UIAction::GPG_LICENSE:
        setSpriteGeometry(registry, entity, 576, 192, width, height, 1_ui8);
        break;
    case UIAction::EXIT:
        setSpriteGeometry(registry, entity, 768, 192, width, height, 1_ui8);
        break;
    case UIAction::CLOSE:
        setSpriteGeometry(registry, entity, 192, 192, width, height, 1_ui8);
        break;
    case UIAction::MENU:
        setSpriteGeometry(registry, entity, 576, 384, width, height, 1_ui8);
        break;
    case UIAction::MENU_CLOSE_UP:
        setSpriteGeometry(registry, entity, 384, 192, width, height, 1_ui8);
        break;
    case UIAction::MENU_CLOSE_DOWN:
        setSpriteGeometry(registry, entity, 0, 192, width, height, 1_ui8);
        break;
    case UIAction::RESTART:
        setSpriteGeometry(registry, entity, 384, 576, width, height, 1_ui8);
        break;
    case UIAction::SHARE:
        setSpriteGeometry(registry, entity, 0, 768, width, height, 2_ui8);
        break;
    case UIAction::SETTINGS:
        setSpriteGeometry(registry, entity, 576, 576, width, height, 1_ui8);
        break;
    case UIAction::SHOP:
    case UIAction::SUPPORT:
        setSpriteGeometry(registry, entity, 384, 768, width, height, 3_ui8);
        break;
    case UIAction::SWITCH_AUDIO:
        setSpriteGeometry(registry, entity, 192, 0, width, height, 2_ui8);
        break;
    case UIAction::SWITCH_VIDEO:
        setSpriteGeometry(registry, entity, 576, 0, width, height, 2_ui8);
        break;
    case UIAction::SWITCH_HAPTIC:
        setSpriteGeometry(registry, entity, 0, 384, width, height, 2_ui8);
        break;
    case UIAction::THE_GAME:
        setSpriteGeometry(registry, entity, 768, 384, width, height, 1_ui8);
        break;
    case UIAction::TRAINING:
        setSpriteGeometry(registry, entity, 768, 576, width, height, 1_ui8);
        break;
    case UIAction::LOGIN:
        setSpriteGeometry(registry, entity, 0, 576, width, height, 2_ui8);
        break;
    case UIAction::LOCKED:
        setSpriteGeometry(registry, entity, 0, 960, width, height, 1_ui8);
        break;
    case UIAction::EASTER_EGG:
    default:
        assert(false);
        break;
    };

    setSpriteSize(registry, entity, width, height);
    registry.assign<BoundingBox>(entity, width, height);
    registry.assign<UIButton>(entity, true, action);

    return entity;
}


entity_type createPopupUIButton(Registry &registry, entity_type parent, UIAction action, int z) {
    auto entity = createUIButton(registry, parent, action, z);
    const auto &sprite = registry.get<Sprite>(entity);
    setPopupUIButtonSize(registry, entity, sprite.w, sprite.h);
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


entity_type createBoxBorder(Registry &registry, entity_type parent, BoxBorderType type, int size, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("ui/win_hud");
    auto entity = createSprite(registry, parent, handle, z);
    const auto sz = 5;

    switch(type) {
    case BoxBorderType::BOX_1_TOP:
        setSpriteGeometry(registry, entity, 720, 0, 436, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_1_BOTTOM:
        setSpriteGeometry(registry, entity, 720, 418, 436, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_1_LEFT:
        setSpriteGeometry(registry, entity, 720, 0, sz, 423, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_1_RIGHT:
        setSpriteGeometry(registry, entity, 1151, 0, sz, 423, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_2_TOP:
        setSpriteGeometry(registry, entity, 720, 423, 436, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_2_BOTTOM:
        setSpriteGeometry(registry, entity, 720, 614, 436, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_2_LEFT:
        setSpriteGeometry(registry, entity, 720, 423, sz, 196, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_2_RIGHT:
        setSpriteGeometry(registry, entity, 1151, 423, sz, 196, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_3_TOP:
        setSpriteGeometry(registry, entity, 720, 619, 356, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_3_BOTTOM:
        setSpriteGeometry(registry, entity, 720, 917, 356, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_3_LEFT:
        setSpriteGeometry(registry, entity, 720, 619, sz, 303, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_3_RIGHT:
        setSpriteGeometry(registry, entity, 1071, 619, sz, 303, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_4_TOP:
        setSpriteGeometry(registry, entity, 720, 922, 437, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_4_BOTTOM:
        setSpriteGeometry(registry, entity, 720, 1087, 437, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_4_LEFT:
        setSpriteGeometry(registry, entity, 720, 922, sz, 170, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_4_RIGHT:
        setSpriteGeometry(registry, entity, 1152, 922, sz, 170, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_5_TOP:
        setSpriteGeometry(registry, entity, 720, 1092, 276, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_5_BOTTOM:
        setSpriteGeometry(registry, entity, 720, 1363, 276, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_5_LEFT:
        setSpriteGeometry(registry, entity, 720, 1092, sz, 276, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_5_RIGHT:
        setSpriteGeometry(registry, entity, 991, 1092, sz, 276, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_6_TOP:
        setSpriteGeometry(registry, entity, 720, 1368, 356, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_6_BOTTOM:
        setSpriteGeometry(registry, entity, 720, 1533, 356, sz, 1_ui8);
        setSpriteSize(registry, entity, size, sz);
        break;
    case BoxBorderType::BOX_6_LEFT:
        setSpriteGeometry(registry, entity, 720, 1368, sz, 170, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    case BoxBorderType::BOX_6_RIGHT:
        setSpriteGeometry(registry, entity, 1071, 1368, sz, 170, 1_ui8);
        setSpriteSize(registry, entity, sz, size);
        break;
    }

    return entity;
}


entity_type createVerticalProgressBar(Registry &registry, entity_type parent, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("ui/win_hud");
    auto entity = createSprite(registry, parent, handle, z);

    setSpriteGeometry(registry, entity, 0, 1280, 512, 64, 11_ui8);
    setSpriteSize(registry, entity, logicalWidth, 40);
    registry.assign<VerticalProgressBar>(entity);
    // rotate it to get a vertical bar
    registry.get<Renderable>(entity).angle = -90;
    // it's a shame, the model for the progress bar is vertical :-)
    registry.get<Sprite>(entity).cols = 1_ui8;

    return entity;
}


}
