#include <cassert>
#include "../common/ease.h"
#include "../factory/common.h"
#include "../factory/play_factory.h"
#include "../locator/locator.hpp"
#include "spawner.h"


namespace gamee {


void Spawner::spawnFromTop(Registry &registry, entity_type entity) {
    const auto &sprite = registry.get<Sprite>(entity);

    const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (sprite.w / 2);
    const int y = -sprite.h + 1;

    const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
    const float impulseY = gravity / 1000;

    setPos(registry, entity, x, y);
    registry.assign<Movement>(entity, gravity, impulseX, impulseY);
}


void Spawner::spawnFromBottom(Registry &registry, entity_type entity) {
    const auto &sprite = registry.get<Sprite>(entity);

    const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (sprite.w / 2);
    const int y = logicalHeight - 1;

    const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
    const float impulseY = -1.f * bottomImpulseY * (generator() / (4.f * generator.max()) + .65f);

    setPos(registry, entity, x, y);
    registry.assign<Movement>(entity, gravity, impulseX, impulseY);
}


void Spawner::spawnFromLeft(Registry &registry, entity_type entity) {
    const auto &sprite = registry.get<Sprite>(entity);

    const int x = -sprite.w + 1;
    const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

    const float impulseX = sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
    const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

    setPos(registry, entity, x, y);
    registry.assign<Movement>(entity, gravity, impulseX, impulseY);
}


void Spawner::spawnFromRight(Registry &registry, entity_type entity) {
    const int x = logicalWidth - 1;
    const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

    const float impulseX = -sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
    const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

    setPos(registry, entity, x, y);
    registry.assign<Movement>(entity, gravity, impulseX, impulseY);
}


void Spawner::maybeRotate(Registry &registry, entity_type entity) {
    delta_type duration = 10000_ui32 + generator() % 10000_ui32;

    if(generator() > (generator.max() / 2)) {
        registry.assign<RotationAnimation>(entity, 0.f, 360.f, duration);
    } else {
        registry.assign<RotationAnimation>(entity, 360.f, 0.f, duration);
    }
}


void Spawner::spawnMessage(Registry &registry, SDLTextureHandle handle, float x, float y) {
    auto entity = createSprite(registry, handle, 190);
    setPos(registry, entity, x - handle->width() / 2.f, y - handle->height() / 2.f);

    registry.assign<Movement>(entity, 0.f, 0.f, scoreDrift);
    registry.assign<FadeAnimation>(entity, 255, 0, 2000_ui32);
    registry.assign<SizeAnimation>(entity, 0, 0, handle->width(), handle->height(), 1000_ui32, 0_ui32, &easeOutElastic);
    registry.assign<DestroyLater>(entity, 2000_ui32);

    // adjust position if requires
    auto &transform = registry.get<Transform>(entity);
    const auto left = transform.x;
    const auto right = transform.x + handle->width();
    const auto bottom = transform.y + handle->height();
    transform.x -= (left < 0.f) ? left : 0.f;
    transform.x -= (right > logicalScreen.w) ? (right - logicalScreen.w) : 0.f;
    transform.y -= (bottom > logicalScreen.h) ? (bottom - logicalScreen.h) : 0.f;
}


Spawner::Spawner()
    : generator{std::random_device{}()}
{}


void Spawner::spawnFaceBottom(Registry &registry, Uint16 smash, Uint16 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, zValue    );
    registry.assign<Destroyable>(entity);
    spawnFromBottom(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnFaceTop(Registry &registry, Uint16 smash, Uint16 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, zValue);
    registry.assign<Destroyable>(entity);
    spawnFromTop(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnFaceLeft(Registry &registry, Uint16 smash, Uint16 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, zValue);
    registry.assign<Destroyable>(entity);
    spawnFromLeft(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnFaceRight(Registry &registry, Uint16 smash, Uint16 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, zValue);
    registry.assign<Destroyable>(entity);
    spawnFromRight(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemBottom(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, zValue);
    registry.assign<Destroyable>(entity);
    spawnFromBottom(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemTop(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, zValue);
    registry.assign<Destroyable>(entity);
    spawnFromTop(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemLeft(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, zValue);
    registry.assign<Destroyable>(entity);
    spawnFromLeft(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemRight(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, zValue);
    registry.assign<Destroyable>(entity);
    spawnFromRight(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnSmashScore(Registry &registry, Uint16 score, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();

    switch(score) {
    case 5:
        spawnMessage(registry, textureCache.handle("str/smash/all/5"), x, y);
        break;
    case 10:
        spawnMessage(registry, textureCache.handle("str/smash/all/10"), x, y);
        break;
    case 20:
        spawnMessage(registry, textureCache.handle("str/smash/all/20"), x, y);
        break;
    case 25:
        spawnMessage(registry, textureCache.handle("str/smash/all/25"), x, y);
        break;
    case 50:
        spawnMessage(registry, textureCache.handle("str/smash/default/50"), x, y);
        break;
    case 100:
        spawnMessage(registry, textureCache.handle("str/smash/default/100"), x, y);
        break;
    case 200:
        spawnMessage(registry, textureCache.handle("str/smash/default/200"), x, y);
        break;
    case 250:
        spawnMessage(registry, textureCache.handle("str/smash/default/250"), x, y);
        break;
    case 500:
        spawnMessage(registry, textureCache.handle("str/smash/rich/500"), x, y);
        break;
    case 1000:
        spawnMessage(registry, textureCache.handle("str/smash/rich/1000"), x, y);
        break;
    case 2000:
        spawnMessage(registry, textureCache.handle("str/smash/rich/2000"), x, y);
        break;
    case 2500:
        spawnMessage(registry, textureCache.handle("str/smash/rich/2500"), x, y);
        break;
    default:
        // does nothing (ie value = 0)
        break;
    }
}


void Spawner::spawnMissScore(Registry &registry, Uint16 score, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();

    switch(score) {
    case 10:
        spawnMessage(registry, textureCache.handle("str/miss/10"), x, y);
        break;
    case 20:
        spawnMessage(registry, textureCache.handle("str/miss/20"), x, y);
        break;
    case 50:
        spawnMessage(registry, textureCache.handle("str/miss/50"), x, y);
        break;
    case 100:
        spawnMessage(registry, textureCache.handle("str/miss/100"), x, y);
        break;
    default:
        // does nothing (ie value = 0)
        break;
    }
}


void Spawner::spawnBonus(Registry &registry, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();
    spawnMessage(registry, textureCache.handle("str/smash/bonus/x10"), x, y);
}


void Spawner::spawnExplosion(Registry &registry, float x, float y) {
    auto entity = createExplosion(registry, 255);
    const auto &sprite = registry.get<Sprite>(entity);
    setPos(registry, entity, x - sprite.w / 2.f, y - sprite.h / 2.f);
    registry.assign<SpriteAnimation>(entity, 750_ui32, 0_ui32, false);
    registry.assign<DestroyLater>(entity, 750_ui32);
}


}
