#include <algorithm>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../factory/common.h"
#include "../factory/play_factory.h"
#include "../factory/spawner.h"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "item_system.h"


namespace gamee {


void ItemSystem::movement(Registry &registry, float mod) {
    for(auto entity: registry.view<Face, Movement>()) {
        registry.get<Movement>(entity).mod = mod;
    };

    for(auto entity: registry.view<Item, Movement>()) {
        registry.get<Movement>(entity).mod = mod;
    };
}


void ItemSystem::fountain(Registry &registry, Spawner &spawner) {
    const auto face = faceBag.get();

    spawner.spawnFaceBottom(registry, 100_ui16, 100_ui16, face);
    spawner.spawnFaceBottom(registry, 100_ui16, 100_ui16, face);
    spawner.spawnFaceBottom(registry, 100_ui16, 100_ui16, face);

    while(registry.size<Face>() < 5) {
        spawner.spawnFaceBottom(registry, 100_ui16, 100_ui16, face);
    }
}


void ItemSystem::message(Registry &registry, SDLTextureHandle handle) {
    auto entity = createInGameMessage(registry, handle, 200);
    const auto &sprite = registry.get<Sprite>(entity);
    setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);
}


void ItemSystem::message(Registry &registry, ItemType type) {
    auto &textureCache = Locator::TextureCache::ref();

    switch(type) {
    case ItemType::BOMB:
        message(registry, textureCache.handle("str/item/bomb"));
        break;
    case ItemType::MONEY:
        message(registry, textureCache.handle("str/item/money"));
        break;
    case ItemType::FOUNTAIN:
        message(registry, textureCache.handle("str/item/fountain"));
        break;
    case ItemType::SLOW_DOWN:
        message(registry, textureCache.handle("str/item/slow"));
        break;
    case ItemType::SPEED_UP:
        message(registry, textureCache.handle("str/item/hurry"));
        break;
    default:
        // random is replaced on click, so no message required
        break;
    }
}


ItemSystem::ItemSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
}


ItemSystem::~ItemSystem() {
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


delta_type ItemSystem::toRemaining(ItemType type) {
    switch(type) {
    case ItemType::BOMB:
    case ItemType::MONEY:
    case ItemType::FOUNTAIN:
        return 1_ui32;
    case ItemType::SLOW_DOWN:
        return 5000_ui32;
    case ItemType::SPEED_UP:
        return 3000_ui32;
    default:
        // random is replaced on click, it has not a duration
        return 0_ui32;
    }
}


void ItemSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void ItemSystem::update(Registry &registry, Spawner &spawner, delta_type delta) {
    auto view = registry.view<Item, Transform, BoundingBox>();

    view.each([&, this](auto entity, const auto &item, const auto &transform, const auto &box) {
        const auto area = transformToPosition(registry, entity, transform) * box;

        if(SDL_HasIntersection(&logicalScreen, &area)) {
            if(dirty && registry.has<Destroyable>(entity) && SDL_PointInRect(&coord, &area)) {
                curr = item.type;

                while(curr == ItemType::RANDOM) {
                    curr = itemBag.get();
                }

                remaining = toRemaining(curr);

                const auto x = area.x + area.w / 2;
                const auto y = area.y + area.h / 2;

                message(registry, curr);
                spawner.spawnExplosion(registry, x, y);
                registry.destroy(entity);

                auto &haptic = Locator::Haptic::ref();
                haptic.rumble(RumbleEffect::SUPER_HARD);
            }
        } else {
            registry.destroy(entity);
        }
    });

    if(remaining) {
        auto &dispatcher = Locator::Dispatcher::ref();

        switch(curr) {
        case ItemType::FOUNTAIN:
            fountain(registry, spawner);
            break;
        case ItemType::SLOW_DOWN:
            movement(registry, .3f);
            break;
        case ItemType::SPEED_UP:
            movement(registry, 1.8f);
            break;
        case ItemType::BOMB:
            dispatcher.enqueue<BonusEvent>(BonusEvent::Type::SMASH_ALL);
            break;
        case ItemType::MONEY:
            dispatcher.enqueue<BonusEvent>(BonusEvent::Type::I_AM_RICH);
            break;
        default:
            // random is replaced on click, so nothing to do here
            break;
        }
    } else {
        // resets movement if curr was speed up
        movement(registry, 1.f);
    }

    remaining -= std::min(remaining, delta);
    dirty = false;
}



}
