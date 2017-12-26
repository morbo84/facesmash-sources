#include "../component/component.hpp"
#include "path_updater_system.h"


namespace gamee {


void PathUpdaterSystem::update(Registry& registry, delta_type delta) {
    auto view = registry.view<Parabola, Transform>(); // TODO: fill in the spaces...

    view.each([delta](auto, auto &parabola, auto &transform) {
        transform.x += parabola.vx * delta;
        transform.y += parabola.vy * delta;
        parabola.vy += parabola.g * delta;
    });
}


}
