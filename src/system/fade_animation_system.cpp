#include "../component/component.hpp"
#include "fade_animation_system.h"


namespace gamee {


void FadeAnimationSystem::update(Registry &registry, delta_type delta) {
    auto view = registry.view<FadeAnimation, Renderable>();

    view.each([delta, &registry](auto entity, auto &animation, auto &renderable) {
        animation.elapsed += delta;

        if(animation.elapsed < animation.duration) {
            const float mul = 1.f * animation.elapsed / animation.duration;
            renderable.alpha = animation.from + (mul * (animation.to - animation.from));
        } else {
            renderable.alpha = animation.to;
            registry.remove<FadeAnimation>(entity);
        }
    });
}


}
