#include <cmath>
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


void AnimationSystem::horizontalAnimation(Registry &registry, delta_type delta) {
    auto view = registry.view<HorizontalAnimation, Transform>();

    view.each([delta, &registry](auto entity, auto &animation, auto &transform) {
        animation.elapsed += delta;

        if(animation.elapsed < animation.duration) {
            transform.x = animation.ease(animation.elapsed, animation.duration, animation.from, animation.to);
        } else {
            transform.x = animation.to;
            registry.remove<HorizontalAnimation>(entity);
        }
    });
}


void AnimationSystem::verticalAnimation(Registry &registry, delta_type delta) {
    auto view = registry.view<VerticalAnimation, Transform>();

    view.each([delta, &registry](auto entity, auto &animation, auto &transform) {
        animation.elapsed += delta;

        if(animation.elapsed < animation.duration) {
            transform.y = animation.ease(animation.elapsed, animation.duration, animation.from, animation.to);
        } else {
            transform.y = animation.to;
            registry.remove<VerticalAnimation>(entity);
        }
    });
}


void AnimationSystem::update(Registry &registry, delta_type delta) {
    fadeAnimation(registry, delta);
    rotationAnimation(registry, delta);
    spriteAnimation(registry, delta);
    horizontalAnimation(registry, delta);
    verticalAnimation(registry, delta);
}


}
