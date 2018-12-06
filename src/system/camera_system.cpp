#include "../component/component.hpp"
#include "camera_system.h"


namespace gamee {


void CameraSystem::update(Registry &registry, delta_type delta) {
    const auto cameraEntity = *registry.view<Camera>().begin();
    auto &camera = registry.get<Camera>(cameraEntity);

    if(camera.remaining) {
        auto &transform = registry.get<Transform>(cameraEntity);

        camera.remaining -= std::min(camera.remaining, delta);

        if(0 == camera.remaining) {
            transform.x = 0.f;
        } else {
            transform.x = transform.x > 0 ? -camera.shake : +camera.shake;
        }
    }
}


}
