#include <cassert>
#include "../locator/locator.hpp"
#include "factory.h"


namespace gamee {


int Factory::toZ(FaceType type) {
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


int Factory::toZ(ItemType type) {
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


void Factory::addScore(Registry &registry, SDLTextureHandle handle, float x, float y) {
    auto entity = registry.create();

    registry.assign<Renderable>(entity, 0.f, 190);
    registry.assign<Movement>(entity, 0.f, 0.f, scoreDrift);
    registry.assign<FadeAnimation>(entity, 255, 0, 2000_ui32);
    registry.assign<DestroyLater>(entity, 2000_ui32);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());

    auto &transform = registry.assign<Transform>(entity, entity, x - handle->width() / 2.f, y - handle->height() / 2.f);
    const auto left = transform.x;
    const auto right = transform.x + handle->width();
    const auto bottom = transform.y + handle->height();
    transform.x -= (left < 0.f) ? left : 0.f;
    transform.x -= (right > logicalScreen.w) ? (right - logicalScreen.w) : 0.f;
    transform.y -= (bottom > logicalScreen.h) ? (bottom - logicalScreen.h) : 0.f;
}


Factory::Factory()
    : generator{std::random_device{}()}
{}


void Factory::spawnFaceBottom(Registry &registry, UInt8 smash, UInt8 miss, FaceType face) {
    spawnFaceOrItemBottom(registry, Face{smash, miss, face});
}


void Factory::spawnFaceTop(Registry &registry, UInt8 smash, UInt8 miss, FaceType face) {
    spawnFaceOrItemTop(registry, Face{smash, miss, face});
}


void Factory::spawnFaceLeft(Registry &registry, UInt8 smash, UInt8 miss, FaceType face) {
    spawnFaceOrItemLeft(registry, Face{smash, miss, face});
}


void Factory::spawnFaceRight(Registry &registry, UInt8 smash, UInt8 miss, FaceType face) {
    spawnFaceOrItemRight(registry, Face{smash, miss, face});
}


void Factory::spawnItemBottom(Registry &registry, ItemType item) {
    spawnFaceOrItemBottom(registry, Item{item});
}


void Factory::spawnItemTop(Registry &registry, ItemType item) {
    spawnFaceOrItemTop(registry, Item{item});
}


void Factory::spawnItemLeft(Registry &registry, ItemType item) {
    spawnFaceOrItemLeft(registry, Item{item});
}


void Factory::spawnItemRight(Registry &registry, ItemType item) {
    spawnFaceOrItemRight(registry, Item{item});
}


void Factory::spawnSmashScore(Registry &registry, UInt8 score, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();

    switch(score) {
    case 50:
        addScore(registry, textureCache.handle("smash/50"), x, y);
        break;
    case 100:
        addScore(registry, textureCache.handle("smash/100"), x, y);
        break;
    case 200:
        addScore(registry, textureCache.handle("smash/200"), x, y);
        break;
    case 250:
        addScore(registry, textureCache.handle("smash/250"), x, y);
        break;
    default:
        // does nothing (ie value = 0)
        break;
    }
}


void Factory::spawnMissScore(Registry &registry, UInt8 score, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();

    switch(score) {
    case 10:
        addScore(registry, textureCache.handle("miss/10"), x, y);
        break;
    case 20:
        addScore(registry, textureCache.handle("miss/20"), x, y);
        break;
    case 50:
        addScore(registry, textureCache.handle("miss/50"), x, y);
        break;
    case 100:
        addScore(registry, textureCache.handle("miss/100"), x, y);
        break;
    default:
        // does nothing (ie value = 0)
        break;
    }
}


void Factory::spawnExplosion(Registry &registry, float x, float y) {
    auto &textureCache = Locator::TextureCache::ref();
    auto entity = registry.create();

    registry.assign<Sprite>(entity, textureCache.handle("game/explosion"), 192, 192, 192, 192, 0, 0, 20_ui8, 5_ui8);
    registry.assign<Renderable>(entity, 0.f, 255);
    registry.assign<Transform>(entity, entity, x, y);
    registry.assign<SpriteAnimation>(entity, 1000_ui32, 0_ui32, false);
    registry.assign<DestroyLater>(entity, 1000_ui32);
}


}
