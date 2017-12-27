#include "../component/component.hpp"
#include "destroy_later_system.h"


namespace gamee {


void DestroyLaterSystem::update(Registry &registry, delta_type delta) {
    auto view = registry.view<DestroyLater>();

    view.each([&registry, delta](auto entity, auto &later) {
        later.elapsed += delta;

        if(later.elapsed > later.delay) {
            registry.destroy(entity);
        }
    });
}


}
