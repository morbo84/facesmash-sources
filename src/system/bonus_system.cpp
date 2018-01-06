#include <cassert>
#include <algorithm>
#include "../component/component.hpp"
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "bonus_system.h"


namespace gamee {


void BonusSystem::update(Registry &registry) {
    if(registry.has<LetsPlay>()) {
        assert(registry.has<BonusSmash>());

        auto &bonus = registry.get<BonusSmash>();

        if(bonus.dirty) {
            auto entity = registry.attachee<BonusSmash>();

            assert(registry.has<Transform>(entity));
            assert(registry.has<Sprite>(entity));

            // update bonus label and show it
            registry.accomodate<FadeAnimation>(entity, 255, 0, 2000_ui32);
            registry.get<Transform>(entity).x = logicalWidth / 2.f - registry.get<Sprite>(entity).handle->width() / 2.f;

            // shake the camera
            registry.remove<CameraShake>();
            registry.attach<CameraShake>(registry.attachee<Camera>());

            bonus.dirty = false;
        }
    }
}


}
