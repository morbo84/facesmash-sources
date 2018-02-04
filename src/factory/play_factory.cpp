#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "common.h"
#include "play_factory.h"


namespace gamee {


entity_type createExplosion(Registry &registry, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("game/explosion");

    const auto width = handle->width() / 5;
    const auto height = handle->height() / 3;

    auto entity = createSprite(registry, handle, z);
    setSpriteGeometry(registry, entity, 0, 0, width, height, 15_ui8, 3_ui8);

    return entity;
}


entity_type createFace(Registry &registry, FaceType face, Uint8 smash, Uint8 miss, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("game/emoji");
    auto entity = createSprite(registry, handle, z);

    const auto width = handle->width() / 3;
    const auto height = handle->height() / 2;

    switch(face) {
    case FaceType::ANGRY:
        setSpriteGeometry(registry, entity, 0, 0, width, height, 1_ui8, 1_ui8);
        break;
    case FaceType::DISGUSTED:
        setSpriteGeometry(registry, entity, 120, 0, width, height, 1_ui8, 1_ui8);
        break;
    case FaceType::FEARFUL:
        setSpriteGeometry(registry, entity, 240, 0, width, height, 1_ui8, 1_ui8);
        break;
    case FaceType::HAPPY:
        setSpriteGeometry(registry, entity, 0, 120, width, height, 1_ui8, 1_ui8);
        break;
    case FaceType::SAD:
        setSpriteGeometry(registry, entity, 120, 120, width, height, 1_ui8, 1_ui8);
        break;
    case FaceType::SURPRISED:
        setSpriteGeometry(registry, entity, 240, 120, width, height, 1_ui8, 1_ui8);
        break;
    }

    setSpriteSize(registry, entity, width, height);
    registry.assign<BoundingBox>(entity, width, height);
    registry.assign<Face>(entity, smash, miss, face);

    return entity;
}


entity_type createItem(Registry &registry, ItemType item, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("game/items");
    auto entity = createSprite(registry, handle, z);

    const auto width = handle->width() / 3;
    const auto height = handle->height();

    switch(item) {
    case ItemType::FOUNTAIN:
        setSpriteGeometry(registry, entity, 0, 240, width, height, 1_ui8, 1_ui8);
        break;
    case ItemType::SLOW_DOWN:
        setSpriteGeometry(registry, entity, 0, 120, width, height, 1_ui8, 1_ui8);
        break;
    case ItemType::SPEED_UP:
        setSpriteGeometry(registry, entity, 0, 0, width, height, 1_ui8, 1_ui8);
        break;
    }

    setSpriteSize(registry, entity, width, height);
    registry.assign<BoundingBox>(entity, width, height);
    registry.assign<Item>(entity, item);

    return entity;
}


}
