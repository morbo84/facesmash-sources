#include "../component/component.hpp"
#include "sprite_animation_system.h"


namespace gamee {


void SpriteAnimationSystem::update(Registry &registry, delta_type delta) {
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


}
