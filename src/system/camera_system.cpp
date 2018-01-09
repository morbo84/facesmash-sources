#include "../component/component.hpp"
#include "camera_system.h"


namespace gamee {


void CameraSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<CameraShake>()) {
        auto &transform = registry.get<Transform>(registry.attachee<Camera>());
        auto &shake = registry.get<CameraShake>();

        shake.remaining -= std::min(shake.remaining, delta);

        if(0 == shake.remaining) {
            registry.remove<CameraShake>();
            transform.x = 0.f;
        } else {
            transform.x = transform.x > 0 ? -shake.shake : +shake.shake;
        }
    }
}


}
