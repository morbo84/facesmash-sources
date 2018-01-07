#include <cassert>
#include "util.h"


namespace gamee {


SDL_Point transformToPosition(const Registry &registry, entity_type entity, const Transform &transform) {
    SDL_Point position;
    position.x = transform.x;
    position.y = transform.y;

    const auto *component = &transform;

    while(component->parent != entity) {
        assert(registry.has<Transform>(component->parent));
        component = &registry.get<Transform>(component->parent);
        position.x += component->x;
        position.y += component->y;
    }

    return position;
}


}
