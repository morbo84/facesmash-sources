#include <algorithm>
#include "../component/component.hpp"
#include "destroy_later_system.h"


namespace gamee {


void DestroyLaterSystem::update(Registry &registry, delta_type delta) {
    auto view = registry.view<DestroyLater>();

    view.each([&registry, delta](auto entity, auto &later) {
        later.remaining -= std::min(later.remaining, delta);

        if(0 == later.remaining) {
            registry.destroy(entity);
        }
    });
}


}
