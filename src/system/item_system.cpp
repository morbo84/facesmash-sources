#include <algorithm>
#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
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

    spawner.spawnFaceBottom(registry, 100_ui8, 100_ui8, face);
    spawner.spawnFaceBottom(registry, 100_ui8, 100_ui8, face);
    spawner.spawnFaceBottom(registry, 100_ui8, 100_ui8, face);

    while(registry.size<Face>() < 5) {
        spawner.spawnFaceBottom(registry, 100_ui8, 100_ui8, face);
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
    case ItemType::FOUNTAIN:
        return 1_ui32;
    case ItemType::SLOW_DOWN:
        return 5000_ui32;
    case ItemType::SPEED_UP:
        return 3000_ui32;
    default:
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
                remaining = toRemaining(curr);

                const auto x = area.x + area.w / 2;
                const auto y = area.y + area.h / 2;

                spawner.spawnExplosion(registry, x, y);
                registry.destroy(entity);
            }
        } else {
            registry.destroy(entity);
        }
    });

    if(remaining) {
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
        }
    } else {
        movement(registry, 1.f);
    }

    remaining -= std::min(remaining, delta);
    dirty = false;
}



}
