#include <algorithm>
#include "../component/component.hpp"
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "reward_system.h"


namespace gamee {


void RewardSystem::update(Registry &registry) {
    if(registry.has<Reward>()) {
        auto &reward = registry.get<Reward>();

        if(reward.dirty) {
            auto entity = registry.attachee<Reward>();
            const auto handle = registry.get<Sprite>(entity).handle;

            // update reward label and show it
            registry.accomodate<FadeAnimation>(entity, 255, 0, 2000_ui32);
            registry.accomodate<SizeAnimation>(entity, 0, 0, handle->width(), handle->height(), 1000_ui32, 0_ui32, &easeOutElastic);
            registry.get<Transform>(entity).x = logicalWidth / 2.f - handle->width() / 2.f;

            // shake the camera
            registry.remove<CameraShake>();
            registry.attach<CameraShake>(registry.attachee<Camera>());

            reward.dirty = false;
        }
    }
}


}
