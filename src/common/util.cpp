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


SDLTextureHandle toHandle(FaceType type) {
    switch(type) {
    case FaceType::ANGRY:
        return Locator::TextureCache::ref().handle("face/angry");
    case FaceType::DISGUSTED:
        return Locator::TextureCache::ref().handle("face/disgusted");
    case FaceType::FEARFUL:
        return Locator::TextureCache::ref().handle("face/fearful");
    case FaceType::HAPPY:
        return Locator::TextureCache::ref().handle("face/happy");
    case FaceType::SAD:
        return Locator::TextureCache::ref().handle("face/sad");
    case FaceType::SURPRISED:
        return Locator::TextureCache::ref().handle("face/surprised");
    default:
        assert(false); // suppress warnings
    }
}


SDLTextureHandle toHandle(ItemType type) {
    switch(type) {
    case ItemType::FOUNTAIN:
        return Locator::TextureCache::ref().handle("item/fountain");
    case ItemType::SLOW_DOWN:
        return Locator::TextureCache::ref().handle("item/slow_down");
    case ItemType::SPEED_UP:
        return Locator::TextureCache::ref().handle("item/speed_up");
    default:
        assert(false); // suppress warnings
    }
}


SDLTextureHandle toStrHud(int value) {
    switch (value) {
    case 0:
        return Locator::TextureCache::ref().handle("str/hud/0");
    case 1:
        return Locator::TextureCache::ref().handle("str/hud/1");
    case 2:
        return Locator::TextureCache::ref().handle("str/hud/2");
    case 3:
        return Locator::TextureCache::ref().handle("str/hud/3");
    case 4:
        return Locator::TextureCache::ref().handle("str/hud/4");
    case 5:
        return Locator::TextureCache::ref().handle("str/hud/5");
    case 6:
        return Locator::TextureCache::ref().handle("str/hud/6");
    case 7:
        return Locator::TextureCache::ref().handle("str/hud/7");
    case 8:
        return Locator::TextureCache::ref().handle("str/hud/8");
    case 9:
        return Locator::TextureCache::ref().handle("str/hud/9");
    default:
        assert(false); // suppress warnings
    }
}


#ifdef DEBUG
SDLTextureHandle toStrDebug(int value) {
    switch (value) {
    case 0:
        return Locator::TextureCache::ref().handle("str/debug/0");
    case 1:
        return Locator::TextureCache::ref().handle("str/debug/1");
    case 2:
        return Locator::TextureCache::ref().handle("str/debug/2");
    case 3:
        return Locator::TextureCache::ref().handle("str/debug/3");
    case 4:
        return Locator::TextureCache::ref().handle("str/debug/4");
    case 5:
        return Locator::TextureCache::ref().handle("str/debug/5");
    case 6:
        return Locator::TextureCache::ref().handle("str/debug/6");
    case 7:
        return Locator::TextureCache::ref().handle("str/debug/7");
    case 8:
        return Locator::TextureCache::ref().handle("str/debug/8");
    case 9:
        return Locator::TextureCache::ref().handle("str/debug/9");
    default:
        assert(false); // suppress warnings
    }
}
#endif // DEBUG


}
