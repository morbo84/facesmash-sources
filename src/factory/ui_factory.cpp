#include "../component/component.hpp"
#include "ui_factory.h"


namespace gamee {


void setPos(Registry &registry, entity_type entity, float x, float y) {
    auto &transform = registry.get<Transform>(entity);
    transform.x = x;
    transform.y = y;
}


void setSpriteSize(Registry &registry, entity_type entity, int w, int h) {
    auto &sprite = registry.get<Sprite>(entity);
    sprite.w = w;
    sprite.h = h;
}


void setBoundingBox(Registry &registry, entity_type entity, int x, int y, int w, int h) {
    registry.get<BoundingBox>(entity) = { x, y, w, h };
}


void setY(Registry &registry, entity_type entity, float y) {
    registry.get<Transform>(entity).y = y;
}


entity_type createPanel(Registry &registry, PanelType type, float x, float y, int w, int h) {
    auto entity = registry.create();
    registry.assign<Panel>(entity, w, h, type);
    registry.assign<Transform>(entity, entity, x, y);
    return entity;
}


entity_type createSprite(Registry &registry, entity_type parent, SDLTextureHandle handle, int z) {
    const auto entity = registry.create();
    registry.assign<Renderable>(entity, 0.f, z);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, parent, 0.f, 0.f);
    return entity;
}


entity_type createUIButton(Registry &registry, entity_type parent, SDLTextureHandle handle, SDLTextureHandle label, UIAction action, int z) {
    auto entity = createSprite(registry, parent, handle, z);
    auto child = createSprite(registry, entity, label, 155);

    registry.assign<UIButton>(entity, child, action);

    setSpriteSize(registry, child, 5 * handle->width() / 6, 5 * handle->width() / 6);
    setPos(registry, child, label->width() / 18, label->width() / 18);

    return entity;
}


entity_type createSmashButton(Registry &registry, entity_type parent, SDLTextureHandle handle, FaceType type, int z) {
    auto entity = createSprite(registry, parent, handle, z);
    registry.assign<BoundingBox>(entity, handle->width(), handle->height());
    registry.assign<SmashButton>(entity, type);
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
