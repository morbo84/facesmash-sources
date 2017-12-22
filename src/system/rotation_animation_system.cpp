#include "../component/component.hpp"
#include "rotation_animation_system.h"


namespace gamee {


void RotationAnimationSystem::update(Registry &registry, delta_type delta) {
    auto view = registry.view<RotationAnimation, Renderable>();

    view.each([delta, &registry](auto entity, auto &animation, auto &renderable) {
        animation.elapsed += delta;

        if(animation.elapsed < animation.duration || animation.repeat) {
            animation.elapsed %= animation.duration;
            renderable.angle = animation.from + (animation.elapsed * (animation.to - animation.from)) / animation.duration;
        } else {
            renderable.angle = animation.to;
            registry.remove<RotationAnimation>(entity);
        }
    });
}


}
