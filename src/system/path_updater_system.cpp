#include "../component/component.hpp"
#include "path_updater_system.h"


namespace gamee {


void PathUpdaterSystem::update(Registry& registry, delta_type delta) {
    auto view = registry.view<Parabola, Transform>(); // TODO: fill in the spaces...

    view.each([delta](auto, auto &parabola, auto &transform) {
        parabola.elapsed += delta;
        transform.x = 0.5f * parabola.g * parabola.elapsed * parabola.elapsed + parabola.vy * parabola.elapsed + parabola.y;
        transform.y = parabola.vx * parabola.elapsed + parabola.x;
    });
}


}
