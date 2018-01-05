#include <algorithm>
#include "../component/component.hpp"
#include "movement_system.h"


namespace gamee {


void MovementSystem::update(Registry& registry, delta_type delta) {
    auto view = registry.view<Movement, Transform>();

    view.each([delta](auto, auto &movement, auto &transform) {
        transform.x += movement.velX * movement.mod * delta;
        transform.y += movement.velY * movement.mod * delta;
        movement.velY += movement.gravity * movement.mod * delta;
        movement.velY = std::min(movement.velY, movement.maxVelY);
    });
}


}
