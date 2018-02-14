#include "../component/component.hpp"
#include "camera_system.h"


namespace gamee {


void CameraSystem::update(Registry &registry, delta_type delta) {
    auto &camera = registry.get<Camera>();

    if(camera.remaining) {
        auto &transform = registry.get<Transform>(registry.attachee<Camera>());

        camera.remaining -= std::min(camera.remaining, delta);

        if(0 == camera.remaining) {
            transform.x = 0.f;
        } else {
            transform.x = transform.x > 0 ? -camera.shake : +camera.shake;
        }
    }
}


}
