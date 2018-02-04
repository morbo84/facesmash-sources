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
    switch (value) {
    case 0:
        return Locator::TextureCache::ref().handle("str/0");
    case 1:
        return Locator::TextureCache::ref().handle("str/1");
    case 2:
        return Locator::TextureCache::ref().handle("str/2");
    case 3:
        return Locator::TextureCache::ref().handle("str/3");
    case 4:
        return Locator::TextureCache::ref().handle("str/4");
    case 5:
        return Locator::TextureCache::ref().handle("str/5");
    case 6:
        return Locator::TextureCache::ref().handle("str/6");
    case 7:
        return Locator::TextureCache::ref().handle("str/7");
    case 8:
        return Locator::TextureCache::ref().handle("str/8");
    case 9:
        return Locator::TextureCache::ref().handle("str/9");
    default:
        assert(false);
        // suppress warnings
        return Locator::TextureCache::ref().handle("str/ ");
    }
}


SDLTextureHandle toStrDebug(int value) {
    switch (value) {
    case 0:
        return Locator::TextureCache::ref().handle("str/0");
    case 1:
        return Locator::TextureCache::ref().handle("str/1");
    case 2:
        return Locator::TextureCache::ref().handle("str/2");
    case 3:
        return Locator::TextureCache::ref().handle("str/3");
    case 4:
        return Locator::TextureCache::ref().handle("str/4");
    case 5:
        return Locator::TextureCache::ref().handle("str/5");
    case 6:
        return Locator::TextureCache::ref().handle("str/6");
    case 7:
        return Locator::TextureCache::ref().handle("str/7");
    case 8:
        return Locator::TextureCache::ref().handle("str/8");
    case 9:
        return Locator::TextureCache::ref().handle("str/9");
    default:
        assert(false);
        // suppress warnings
        return Locator::TextureCache::ref().handle("str/ ");
    }
}


}
