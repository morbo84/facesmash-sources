#include "../common/ease.h"
#include "../component/component.hpp"
#include "../factory/spawner.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "training_system.h"


namespace gamee {


void TrainingSystem::showFaceButtons(Registry &registry) {
    registry.view<FaceButton, Sprite, Renderable>().each([&registry](auto entity, auto &button, const auto &sprite, auto &renderable) {
        button.enabled = true;
        renderable.angle = 0.f;
        registry.accomodate<RotationAnimation>(entity, renderable.angle, 720.f, 1500_ui32, 0_ui32, false, &easeOutElastic);
        registry.accomodate<SizeAnimation>(entity, sprite.w, sprite.h, button.w, button.h, 1500_ui32, 0_ui32, &easeOutElastic);
    });
}


void TrainingSystem::hideFaceButtons(Registry &registry) {
    registry.view<FaceButton, Sprite, Renderable>().each([&registry](auto entity, auto &button, const auto &sprite, auto &renderable) {
        button.enabled = false;
        renderable.angle = 0.f;
        registry.accomodate<RotationAnimation>(entity, renderable.angle, 720.f, 1500_ui32, 0_ui32, false, &easeOutCubic);
        registry.accomodate<SizeAnimation>(entity, sprite.w, sprite.h, 0, 0, 500_ui32, 0_ui32, &easeInCubic);
    });
}


void TrainingSystem::startTraining(Registry &registry) {
    // TODO
}


void TrainingSystem::training(Registry &registry, Spawner &spawner, delta_type delta) {
    // TODO
}


void TrainingSystem::stopTraining(Registry &registry) {
    // TODO
}


void TrainingSystem::idle(Registry &registry) {
    // TODO
}


TrainingSystem::TrainingSystem()
    : current{FaceType::HAPPY},
      match{FaceType::ANGRY},
      progress{0},
      remaining{0_ui32},
      state{TrainingState::IDLE}
{
    Locator::Dispatcher::ref().connect<FaceRequest>(this);
    Locator::Dispatcher::ref().connect<FaceEvent>(this);
}


TrainingSystem::~TrainingSystem() {
    Locator::Dispatcher::ref().disconnect<FaceRequest>(this);
    Locator::Dispatcher::ref().disconnect<FaceEvent>(this);
}


void TrainingSystem::receive(const FaceRequest &event) noexcept {
    if(state == TrainingState::IDLE) {
        current = event.type;
        match = current == FaceType::HAPPY ? FaceType::ANGRY : FaceType::HAPPY;
        progress = 0;
        remaining = interval;
        state = TrainingState::START_TRAINING;
    }
}


void TrainingSystem::receive(const FaceEvent &event) noexcept {
    if(state == TrainingState::TRAINING) {
        match = event.type;
    }
}


void TrainingSystem::update(Registry &registry, Spawner &spawner, delta_type delta) {
    if(registry.has<LetsTrain>()) {
        switch(state) {
        case TrainingState::IDLE:
            idle(registry);
            break;
        case TrainingState::START_TRAINING:
            hideFaceButtons(registry);
            startTraining(registry);
            break;
        case TrainingState::STOP_TRAINING:
            showFaceButtons(registry);
            stopTraining(registry);
            break;
        case TrainingState::TRAINING:
            training(registry, spawner, delta);
            break;
        }
    } else if(state != TrainingState::IDLE) {
        showFaceButtons(registry);
        state = TrainingState::IDLE;
        remaining = 0_ui32;
        progress = 0;
    }
}


}
