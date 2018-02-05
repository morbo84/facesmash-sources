#include <cassert>
#include "../common/ease.h"
#include "../factory/common.h"
#include "../factory/play_factory.h"
#include "../locator/locator.hpp"
#include "spawner.h"


namespace gamee {


int Spawner::toZ(FaceType type) {
    switch(type) {
    case FaceType::HAPPY:
        return 186;
    case FaceType::ANGRY:
        return 185;
    case FaceType::SURPRISED:
        return 184;
    case FaceType::SAD:
        return 183;
    case FaceType::DISGUSTED:
        return 182;
    case FaceType::FEARFUL:
        return 181;
    default:
        return 180;
    }
}


int Spawner::toZ(ItemType type) {
    switch(type) {
    case ItemType::FOUNTAIN:
        return 173;
    case ItemType::SLOW_DOWN:
        return 172;
    case ItemType::SPEED_UP:
        return 171;
    default:
        return 170;
    }
}


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


void Spawner::addScore(Registry &registry, SDLTextureHandle handle, float x, float y) {
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


void Spawner::spawnFaceBottom(Registry &registry, Uint8 smash, Uint8 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, toZ(face));
    registry.assign<Destroyable>(entity);
    spawnFromBottom(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnFaceTop(Registry &registry, Uint8 smash, Uint8 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, toZ(face));
    registry.assign<Destroyable>(entity);
    spawnFromTop(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnFaceLeft(Registry &registry, Uint8 smash, Uint8 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, toZ(face));
    registry.assign<Destroyable>(entity);
    spawnFromLeft(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnFaceRight(Registry &registry, Uint8 smash, Uint8 miss, FaceType face) {
    auto entity = createFace(registry, face, smash, miss, toZ(face));
    registry.assign<Destroyable>(entity);
    spawnFromRight(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemBottom(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, toZ(item));
    registry.assign<Destroyable>(entity);
    spawnFromBottom(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemTop(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, toZ(item));
    registry.assign<Destroyable>(entity);
    spawnFromTop(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemLeft(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, toZ(item));
    registry.assign<Destroyable>(entity);
    spawnFromLeft(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnItemRight(Registry &registry, ItemType item) {
    auto entity = createItem(registry, item, toZ(item));
    registry.assign<Destroyable>(entity);
    spawnFromRight(registry, entity);
    maybeRotate(registry, entity);
}


void Spawner::spawnSmashScore(Registry &registry, Uint8 score, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();

    switch(score) {
    case 50:
        addScore(registry, textureCache.handle("str/smash/50"), x, y);
        break;
    case 100:
        addScore(registry, textureCache.handle("str/smash/100"), x, y);
        break;
    case 200:
        addScore(registry, textureCache.handle("str/smash/200"), x, y);
        break;
    case 250:
        addScore(registry, textureCache.handle("str/smash/250"), x, y);
        break;
    default:
        // does nothing (ie value = 0)
        break;
    }
}


void Spawner::spawnMissScore(Registry &registry, Uint8 score, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();

    switch(score) {
    case 10:
        addScore(registry, textureCache.handle("str/miss/10"), x, y);
        break;
    case 20:
        addScore(registry, textureCache.handle("str/miss/20"), x, y);
        break;
    case 50:
        addScore(registry, textureCache.handle("str/miss/50"), x, y);
        break;
    case 100:
        addScore(registry, textureCache.handle("str/miss/100"), x, y);
        break;
    default:
        // does nothing (ie value = 0)
        break;
    }
}


void Spawner::spawnExplosion(Registry &registry, float x, float y) {
    auto entity = createExplosion(registry, 255);
    const auto &sprite = registry.get<Sprite>(entity);
    setPos(registry, entity, x - sprite.w / 2.f, y - sprite.h / 2.f);
    registry.assign<SpriteAnimation>(entity, 1000_ui32, 0_ui32, false);
    registry.assign<DestroyLater>(entity, 1000_ui32);
}


}
