#include <array>
#include <cassert>
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "util.h"


namespace gamee {


int numOfDigits(int value) {
    int count = 0;

    do {
        ++count;
        value /= 10;
    } while(value);

    return count;
}


SDL_Point transformToPosition(const Registry &registry, entity_type entity, const Transform &transform) {
    SDL_Point position;
    position.x = transform.x;
    position.y = transform.y;

    const auto *component = &transform;

    while(component->parent != entity) {
        entity = component->parent;
        component = &registry.get<Transform>(component->parent);
        position.x += component->x;
        position.y += component->y;
    }

    return position;
}


SDLTextureHandle toStrHud(int value) {
    assert(value >= 0);
    assert(value < 10);
    return std::array<SDLTextureHandle, 10>{
        Locator::TextureCache::ref().handle("str/0"),
        Locator::TextureCache::ref().handle("str/1"),
        Locator::TextureCache::ref().handle("str/2"),
        Locator::TextureCache::ref().handle("str/3"),
        Locator::TextureCache::ref().handle("str/4"),
        Locator::TextureCache::ref().handle("str/5"),
        Locator::TextureCache::ref().handle("str/6"),
        Locator::TextureCache::ref().handle("str/7"),
        Locator::TextureCache::ref().handle("str/8"),
        Locator::TextureCache::ref().handle("str/9")
    }[value];
}


SDLTextureHandle toStrDebug(int value) {
    assert(value >= 0);
    assert(value < 10);
    return std::array<SDLTextureHandle, 10>{
        Locator::TextureCache::ref().handle("str/debug/0"),
        Locator::TextureCache::ref().handle("str/debug/1"),
        Locator::TextureCache::ref().handle("str/debug/2"),
        Locator::TextureCache::ref().handle("str/debug/3"),
        Locator::TextureCache::ref().handle("str/debug/4"),
        Locator::TextureCache::ref().handle("str/debug/5"),
        Locator::TextureCache::ref().handle("str/debug/6"),
        Locator::TextureCache::ref().handle("str/debug/7"),
        Locator::TextureCache::ref().handle("str/debug/8"),
        Locator::TextureCache::ref().handle("str/debug/9")
    }[value];
}


}
