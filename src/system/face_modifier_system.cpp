#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "face_modifier_system.h"


namespace gamee {


void FaceModifierSystem::update(Registry &registry, float mod) {
    auto view = registry.view<Movement, FaceSmash>();

    for(auto entity: view) {
        registry.get<Movement>(entity).mod = mod;
    }
}


FaceModifierSystem::FaceModifierSystem()
    : modifier{FaceModifier::NONE},
      remaining{0_ui32}
{
    Locator::Dispatcher::ref().connect<FaceModifierEvent>(this);
}


FaceModifierSystem::~FaceModifierSystem() {
    Locator::Dispatcher::ref().disconnect<FaceModifierEvent>(this);
}


void FaceModifierSystem::receive(const FaceModifierEvent &event) noexcept {
    modifier = event.modifier;
    remaining = event.duration;
}


void FaceModifierSystem::update(Registry &registry, delta_type delta) {
    remaining = delta > remaining ? 0_ui32 : (remaining - delta);

    if(0 == remaining) {
        modifier = FaceModifier::NONE;
    }

    switch(modifier) {
    case FaceModifier::NONE:
        update(registry, 1.f);
        break;
    case FaceModifier::SLOW_DOWN:
        update(registry, .3f);
        break;
    case FaceModifier::SPEED_UP:
        update(registry, 1.5f);
        break;
    }
}



}
