#include "../component/component.hpp"
#include "path_updater_system.h"


namespace gamee {


void PathUpdaterSystem::update(Registry& registry, delta_type delta) {
    auto view = registry.view<Parabola, Transform, Renderable>(); // TODO: fill in the spaces...
    for(auto entityId : view) {
        auto& prbl = registry.get<Parabola>(entityId);
        prbl.t += delta;
        auto& transform = registry.get<Transform>(entityId);
        transform.x = 0.5f * prbl.g * prbl.t * prbl.t + prbl.vy * prbl.t + prbl.p1.y;
        transform.y = prbl.vx * prbl.t + prbl.p1.x;
    }
}


}
