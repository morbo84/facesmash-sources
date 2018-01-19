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


entity_type createUIButton(Registry &registry, entity_type parent, SDLTextureHandle handle, UIAction action, int z) {
    auto entity = registry.create();
    registry.assign<Renderable>(entity, 0.f, z);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, parent, 0.f, 0.f);
    registry.assign<UIButton>(entity, action);
    return entity;
}


entity_type createSmashButton(Registry &registry, entity_type parent, SDLTextureHandle handle, FaceType type, int z) {
    auto entity = registry.create();
    registry.assign<SmashButton>(entity, type);
    registry.assign<Transform>(entity, parent, 0.f, 0.f);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<BoundingBox>(entity, handle->width(), handle->height());
    registry.assign<Renderable>(entity, 0.f, z);
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