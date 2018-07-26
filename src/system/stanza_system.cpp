#include <cassert>
#include "../component/component.hpp"
#include "../common/util.h"
#include "../factory/common.h"
#include "../factory/play_factory.h"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "stanza_system.h"


namespace gamee {


void StanzaSystem::detectInGameItemTutorial(Registry &registry, Stanza &stanza) {
    for(const auto entity: registry.view<Item, Transform, Movement, BoundingBox>()) {
        const auto &transform = registry.get<Transform>(entity);
        const auto &movement = registry.get<Movement>(entity);
        const auto &box = registry.get<BoundingBox>(entity);

        const auto area = transformToPosition(registry, entity, transform) * box;
        const auto intersect = SDL_HasIntersection(&stanza, &area);

        if(intersect) {
            action_fn = &StanzaSystem::slowDownEverything;
            remaining = duration;
            target = entity;

            const auto tutorialHandle = Locator::TextureCache::ref().handle("str/tutorial/touchme");
            auto tutorialLabel = createLightningMessage(registry, tutorialHandle, 200);

            const auto x = transform.x + (box.w - tutorialHandle->width()) / 2;
            const auto y = transform.y + (movement.velY > 0 ? 0 : box.h) - tutorialHandle->height() / 2;
            setPos(registry, tutorialLabel, x, y);

            stanza.enabled = false;
            break;
        }
    }
}


void StanzaSystem::slowDownEverything(Registry &registry) {
    for(auto entity: registry.view<Face, Movement>()) {
        registry.get<Movement>(entity).mod = entity == target ? .05f : .75f;
    };

    for(auto entity: registry.view<Item, Movement>()) {
        registry.get<Movement>(entity).mod = entity == target ? .05f : .75f;
    };
}


void StanzaSystem::update(Registry &registry, delta_type delta) {
    remaining -= registry.valid(target) ? std::min(remaining, delta) : remaining;

    if(remaining) {
        (this->*action_fn)(registry);
    } else {
        target = entt::null;

        registry.view<Stanza>().each([&registry, this](const auto, auto &stanza) {
            if(stanza.enabled) {
                switch(stanza.type) {
                case StanzaType::IN_GAME_ITEM_TUTORIAL:
                    detectInGameItemTutorial(registry, stanza);
                    break;
                default:
                    assert(false);
                    break;
                }
            }
        });
    }
}


}
