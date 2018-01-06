#include "../component/component.hpp"
#include "animation_system.h"


namespace gamee {


void AnimationSystem::fadeAnimation(Registry &registry, delta_type delta) {
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


void AnimationSystem::rotationAnimation(Registry &registry, delta_type delta) {
    auto view = registry.view<RotationAnimation, Renderable>();

    view.each([delta, &registry](auto entity, auto &animation, auto &renderable) {
        animation.elapsed += delta;

        if(animation.elapsed < animation.duration || animation.repeat) {
            animation.elapsed %= animation.duration;
            const float mul = 1.f * animation.elapsed / animation.duration;
            renderable.angle = animation.from + (mul * (animation.to - animation.from));
        } else {
            renderable.angle = animation.to;
            registry.remove<RotationAnimation>(entity);
        }
    });
}


void AnimationSystem::spriteAnimation(Registry &registry, delta_type delta) {
    auto view = registry.view<SpriteAnimation, Sprite>();

    view.each([delta, &registry](auto entity, auto &animation, auto &sprite) {
        animation.elapsed += delta;

        if(animation.elapsed < animation.duration || animation.repeat) {
            animation.elapsed %= animation.duration;
            sprite.frame = (animation.elapsed * sprite.frames) / animation.duration;
        } else {
            registry.remove<SpriteAnimation>(entity);
        }
    });
}


void AnimationSystem::update(Registry &registry, delta_type delta) {
    fadeAnimation(registry, delta);
    rotationAnimation(registry, delta);
    spriteAnimation(registry, delta);
}


}
