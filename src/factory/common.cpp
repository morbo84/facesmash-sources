#include "../component/component.hpp"
#include "common.h"


namespace gamee {


static void internalCreateSprite(Registry &registry, entity_type entity, entity_type parent, SDLTextureHandle handle, int z) {
    registry.assign<Renderable>(entity, 0.f, z);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<Transform>(entity, parent, 0.f, 0.f);
}


entity_type createSprite(Registry &registry, entity_type parent, SDLTextureHandle handle, int z) {
    const auto entity = registry.create();
    internalCreateSprite(registry, entity, parent, handle, z);
    return entity;
}


entity_type createSprite(Registry &registry, SDLTextureHandle handle, int z) {
    const auto entity = registry.create();
    internalCreateSprite(registry, entity, entity, handle, z);
    return entity;
}


void setPos(Registry &registry, entity_type entity, float x, float y) {
    auto &transform = registry.get<Transform>(entity);
    transform.x = x;
    transform.y = y;
}


void setSpriteGeometry(Registry &registry, entity_type entity, int x, int y, int width, int height, Uint8 frames, Uint8 rows) {
    auto &sprite = registry.get<Sprite>(entity);
    sprite.x = x;
    sprite.y = y;
    sprite.width = width;
    sprite.height = height;
    sprite.frames = frames;
    sprite.rows = rows;
}


void setSpriteSize(Registry &registry, entity_type entity, int w, int h) {
    auto &sprite = registry.get<Sprite>(entity);
    sprite.w = w;
    sprite.h = h;
}


void setBoundingBox(Registry &registry, entity_type entity, int x, int y, int w, int h) {
    registry.get<BoundingBox>(entity) = { x, y, w, h };
}


void setBoundingBox(Registry &registry, entity_type entity, int w, int h) {
    setBoundingBox(registry, entity, 0, 0, w, h);
}


}
