#include "../component/component.hpp"
#include "../locator/locator.hpp"
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


entity_type createFaceBlueprint(Registry &registry, FaceType type, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("game/emoji");

    const auto width = handle->width() / 3;
    const auto height = handle->height() / 2;

    auto entity = createSprite(registry, handle, z);
    setSpriteSize(registry, entity, width, height);

    switch(type) {
    case FaceType::ANGRY:
        setSpriteGeometry(registry, entity, 0, 0, width, height, 1_ui8);
        break;
    case FaceType::DISGUSTED:
        setSpriteGeometry(registry, entity, 120, 0, width, height, 1_ui8);
        break;
    case FaceType::FEARFUL:
        setSpriteGeometry(registry, entity, 240, 0, width, height, 1_ui8);
        break;
    case FaceType::HAPPY:
        setSpriteGeometry(registry, entity, 0, 120, width, height, 1_ui8);
        break;
    case FaceType::SAD:
        setSpriteGeometry(registry, entity, 120, 120, width, height, 1_ui8);
        break;
    case FaceType::SURPRISED:
        setSpriteGeometry(registry, entity, 240, 120, width, height, 1_ui8);
        break;
    }

    return entity;
}


entity_type createItemBlueprint(Registry &registry, ItemType type, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("game/items");

    const auto width = handle->width() / 3;
    const auto height = handle->height();

    auto entity = createSprite(registry, handle, z);
    setSpriteSize(registry, entity, width, height);

    switch(type) {
    case ItemType::FOUNTAIN:
        setSpriteGeometry(registry, entity, 240, 0, width, height, 1_ui8);
        break;
    case ItemType::SLOW_DOWN:
        setSpriteGeometry(registry, entity, 120, 0, width, height, 1_ui8);
        break;
    case ItemType::SPEED_UP:
        setSpriteGeometry(registry, entity, 0, 0, width, height, 1_ui8);
        break;
    }

    return entity;
}



void setPos(Registry &registry, entity_type entity, float x, float y) {
    auto &transform = registry.get<Transform>(entity);
    transform.x = x;
    transform.y = y;
}


void setSpriteGeometry(Registry &registry, entity_type entity, int x, int y, int width, int height, Uint8 frames) {
    auto &sprite = registry.get<Sprite>(entity);
    sprite.x = x;
    sprite.y = y;
    sprite.width = width;
    sprite.height = height;
    sprite.frames = frames;
    // horizontal sprites by default
    sprite.cols = frames;
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
