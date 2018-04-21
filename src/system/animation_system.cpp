#include <cmath>
#include "../common/ease.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "animation_system.h"


namespace gamee {


void AnimationSystem::fadeAnimation(Registry &registry, delta_type delta) {
    auto view = registry.view<FadeAnimation, Renderable>();

    view.each([delta, &registry](auto entity, auto &animation, auto &renderable) {
        animation.elapsed += delta;

        if(animation.elapsed < animation.duration) {
            renderable.alpha = animation.ease(animation.elapsed, animation.duration, animation.from, animation.to);
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
            renderable.angle = animation.ease(animation.elapsed, animation.duration, animation.from, animation.to);
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


void AnimationSystem::sizeAnimation(Registry &registry, delta_type delta) {
    auto view = registry.view<SizeAnimation, Sprite, Transform>();

    view.each([delta, &registry](auto entity, auto &animation, auto &sprite, auto &transform) {
        animation.elapsed += delta;

        const int xC = transform.x + sprite.w / 2;
        const int yC = transform.y + sprite.h / 2;

        if(animation.elapsed < animation.duration) {
            sprite.w = animation.ease(animation.elapsed, animation.duration, animation.fromW, animation.toW);
            sprite.h = animation.ease(animation.elapsed, animation.duration, animation.fromH, animation.toH);
        } else {
            sprite.w = animation.toW;
            sprite.h = animation.toH;
            registry.remove<SizeAnimation>(entity);
        }

        transform.x = xC - sprite.w / 2;
        transform.y = yC - sprite.h / 2;

        if(registry.has<BoundingBox>(entity)) {
            auto &box = registry.get<BoundingBox>(entity);
            box.x = (sprite.w - box.w) / 2;
            box.y = (sprite.h - box.h) / 2;
        }
    });
}


void AnimationSystem::pulseAnimation(Registry &registry, delta_type delta) {
    auto view = registry.view<PulseAnimation, Renderable, Sprite, Transform>();

    view.each([&registry, delta](auto entity, auto &animation, const auto &renderable, const auto &sprite, const auto &transform) {
        animation.elapsed += delta;

        if(animation.elapsed > animation.duration) {
            const auto position = transformToPosition(registry, entity, transform);
            auto pulse = registry.create();

            // we don't want to replace current animations
            if(!registry.has<RotationAnimation>(entity)) {
                registry.assign<RotationAnimation>(entity, renderable.angle - animation.angle, renderable.angle, 500_ui32, 0_ui32, false, &easeOutElastic);
            }

            // we don't want to replace current animations
            if(!registry.has<SizeAnimation>(entity)) {
                const int w = sprite.w * (1 - animation.squeeze);
                const int h = sprite.h * (1 - animation.squeeze);
                registry.accommodate<SizeAnimation>(entity, w, h, sprite.w, sprite.h, 1000_ui32, 0_ui32, &easeOutElastic);
            }

            registry.assign<DestroyLater>(pulse, 1000_ui32);
            registry.assign<Renderable>(pulse, renderable);
            registry.assign<Sprite>(pulse, sprite);
            registry.assign<Transform>(pulse, pulse, 1.f * position.x, 1.f * position.y);
            registry.assign<FadeAnimation>(pulse, 99, 0, 1000_ui32, 0_ui32, &easeOutExpo);

            const auto sz = 1.f + animation.length;
            const int w = sprite.w * sz;
            const int h = sprite.h * sz;
            registry.accommodate<SizeAnimation>(pulse, sprite.w, sprite.h, w, h, 1000_ui32, 0_ui32, &easeOutElastic);

            if(animation.repeat) {
                animation.elapsed = 0_ui32;
            } else {
                registry.remove<SpriteAnimation>(entity);
            }
        }
    });
}


void AnimationSystem::update(Registry &registry, delta_type delta) {
    fadeAnimation(registry, delta);
    rotationAnimation(registry, delta);
    spriteAnimation(registry, delta);
    horizontalAnimation(registry, delta);
    verticalAnimation(registry, delta);
    sizeAnimation(registry, delta);
    pulseAnimation(registry, delta);
}


}
