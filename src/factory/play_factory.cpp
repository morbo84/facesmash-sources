#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "common.h"
#include "play_factory.h"


namespace gamee {


entity_type createInGameMessage(Registry &registry, SDLTextureHandle handle, int z) {
    auto entity = createSprite(registry, handle, z);
    registry.assign<FadeAnimation>(entity, 255, 0, 1000_ui32, 0_ui32, &easeInExpo);
    registry.assign<SizeAnimation>(entity, 0, 0, handle->width(), handle->height(), 1000_ui32, 0_ui32, &easeOutElastic);
    registry.assign<DestroyLater>(entity, 1000_ui32);
    return entity;
}


entity_type createInTrainingMessage(Registry &registry, SDLTextureHandle handle, int z) {
    auto entity = createSprite(registry, handle, z);
    registry.assign<FadeAnimation>(entity, 255, 0, 3000_ui32, 0_ui32, &easeInCubic);
    registry.assign<SizeAnimation>(entity, 0, 0, handle->width(), handle->height(), 1000_ui32, 0_ui32, &easeOutElastic);
    registry.assign<DestroyLater>(entity, 3000_ui32);
    return entity;
}


entity_type createExplosion(Registry &registry, int z) {
    auto &textureCache = Locator::TextureCache::ref();
    auto handle = textureCache.handle("game/explosion");

    const auto width = handle->width() / 5;
    const auto height = handle->height() / 3;

    auto entity = createSprite(registry, handle, z);
    setSpriteGeometry(registry, entity, 0, 0, width, height, 15_ui8);
    setSpriteSize(registry, entity, width, height);
    registry.get<Sprite>(entity).cols = 5_ui8;

    return entity;
}


entity_type createSmashButton(Registry &registry, FaceType type, int z) {
    auto entity = createFaceBlueprint(registry, type, z);
    const auto &sprite = registry.get<Sprite>(entity);
    registry.assign<BoundingBox>(entity, sprite.w, sprite.h);
    registry.assign<SmashButton>(entity, type);
    return entity;
}


entity_type createFaceButton(Registry &registry, FaceType type, int z) {
    auto entity = createFaceBlueprint(registry, type, z);
    const auto &sprite = registry.get<Sprite>(entity);
    registry.assign<BoundingBox>(entity, sprite.w, sprite.h);
    registry.assign<FaceButton>(entity, true, type, sprite.w, sprite.h);
    return entity;
}


entity_type createFace(Registry &registry, FaceType type, Uint8 smash, Uint8 miss, int z) {
    auto entity = createFaceBlueprint(registry, type, z);
    const auto &sprite = registry.get<Sprite>(entity);
    registry.assign<BoundingBox>(entity, sprite.w, sprite.h);
    registry.assign<Face>(entity, smash, miss, type);
    return entity;
}


entity_type createItem(Registry &registry, ItemType type, int z) {
    auto entity = createItemBlueprint(registry, type, z);
    const auto &sprite = registry.get<Sprite>(entity);
    registry.assign<BoundingBox>(entity, sprite.w, sprite.h);
    registry.assign<Item>(entity, type);
    return entity;
}


}
