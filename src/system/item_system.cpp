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


void ItemSystem::timer(Registry &registry, ItemType type, delta_type delta) {
    if(!registry.empty<Timer>()) {
        auto &timer = *registry.raw<Timer>();

        if(timer.enabled) {
            switch(type) {
            case ItemType::SLOW_DOWN:
                timer.remaining += delta;
                break;
            case ItemType::SPEED_UP:
                timer.remaining -= std::min(timer.remaining, delta);
            default:
                // something went wrong here, give up and that's all
                break;
            }
        }
    }
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
    for(auto entity: registry.view<ItemMessage>()) {
        registry.destroy(entity);
    }

    auto entity = createLastingMessage(registry, handle, 200);
    registry.assign<ItemMessage>(entity);
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


ItemSystem::ItemSystem(): dirty{false}, armageddon{false} {
    Locator::Dispatcher::ref().sink<TouchEvent>().connect<&ItemSystem::onTouch>(this);
    Locator::Dispatcher::ref().sink<ArmageddonEvent>().connect<&ItemSystem::onArmageddon>(this);
}


ItemSystem::~ItemSystem() {
    Locator::Dispatcher::ref().sink<ArmageddonEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<TouchEvent>().disconnect(this);
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


void ItemSystem::onTouch(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void ItemSystem::onArmageddon(const ArmageddonEvent &) noexcept {
    armageddon = true;
    remaining = {};
}


void ItemSystem::update(Registry &registry, Spawner &spawner, delta_type delta) {
    auto view = registry.view<Item, Transform, BoundingBox>();

    view.each([&, this](auto entity, const auto &item, const auto &transform, const auto &box) {
        const auto area = transformToPosition(registry, entity, transform) * box;

        const auto x = area.x + area.w / 2;
        const auto y = area.y + area.h / 2;

        if(SDL_HasIntersection(&logicalScreen, &area)) {
            if(registry.has<Destroyable>(entity)) {
                if(dirty && SDL_PointInRect(&coord, &area)) {
                    curr = item.type;

                    while(curr == ItemType::RANDOM) {
                        curr = itemBag.get();
                    }

                    remaining = toRemaining(curr);

                    message(registry, curr);
                    spawner.spawnExplosion(registry, x, y);
                    registry.destroy(entity);

                    auto &haptic = Locator::Haptic::ref();
                    haptic.rumble(RumbleEffect::SUPER_HARD);
                }
            } else if(armageddon) {
                spawner.spawnExplosion(registry, x, y);
                registry.destroy(entity);
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
            timer(registry, curr, delta/2);
            movement(registry, .3f);
            break;
        case ItemType::SPEED_UP:
            timer(registry, curr, delta/2);
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
    armageddon = false;
    dirty = false;
}



}
