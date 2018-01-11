#include "../common/util.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../factory/factory.h"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "item_system.h"


namespace gamee {


void ItemSystem::movement(Registry &registry, float mod) {
    registry.view<Movement>().each([mod](auto, auto &movement) {
        movement.mod = mod;
    });
}


void ItemSystem::fountain(Registry &registry, Factory &factory) {
    if(registry.size<Face>() < 5) {
        factory.spawnFaceBottom(registry, 100_ui8, 100_ui8);
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
        return 500_ui32;
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


void ItemSystem::update(Registry &registry, Factory &factory, delta_type delta) {
    const SDL_Rect screen = logicalScreen;

    auto view = registry.view<Item, Transform, BoundingBox>();

    view.each([&, this](auto entity, const auto &item, const auto &transform, const auto &box) {
        const auto area = transformToPosition(registry, entity, transform) * box;

        if(SDL_HasIntersection(&screen, &area)) {
            if(dirty && SDL_PointInRect(&coord, &area)) {
                curr = item.type;
                remaining = toRemaining(curr);

                const auto x = area.x + area.w / 2;
                const auto y = area.y + area.h / 2;

                factory.spawnExplosion(registry, x, y);
                registry.destroy(entity);
            }
        } else {
            registry.destroy(entity);
        }
    });

    remaining = delta > remaining ? 0_ui32 : (remaining - delta);

    if(remaining) {
        switch(curr) {
        case ItemType::FOUNTAIN:
            fountain(registry, factory);
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
}



}
