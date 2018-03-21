#include <algorithm>
#include "../common/constants.h"
#include "../common/ease.h"
#include "../component/component.hpp"
#include "../factory/common.h"
#include "../factory/play_factory.h"
#include "../factory/spawner.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "training_system.h"


namespace gamee {


void TrainingSystem::enableFaceButtons(Registry &registry) {
    registry.view<FaceButton, Sprite, Renderable>().each([&registry](auto entity, auto &button, const auto &sprite, auto &renderable) {
        if(!button.enabled) {
            button.enabled = true;
            renderable.angle = 0.f;
            registry.accomodate<RotationAnimation>(entity, renderable.angle, 720.f, 1500_ui32, 0_ui32, false, &easeOutElastic);
            registry.accomodate<SizeAnimation>(entity, sprite.w, sprite.h, button.w, button.h, 1500_ui32, 0_ui32, &easeOutElastic);
        }
    });
}


void TrainingSystem::disableFaceButtons(Registry &registry) {
    registry.view<FaceButton, Sprite, Renderable>().each([&registry](auto entity, auto &button, const auto &sprite, auto &renderable) {
        if(button.enabled) {
            button.enabled = false;
            renderable.angle = 0.f;
            registry.accomodate<RotationAnimation>(entity, renderable.angle, 720.f, 1500_ui32, 0_ui32, false, &easeOutCubic);
            registry.accomodate<SizeAnimation>(entity, sprite.w, sprite.h, 0, 0, 500_ui32, 0_ui32, &easeInCubic);
        }
    });
}


TrainingSystem::TrainingState TrainingSystem::training(Registry &registry, Spawner &spawner, delta_type delta) {
    const bool hurry = remaining < (duration / 2);
    TrainingState next = TrainingState::TRAINING;

    remaining -= std::min(remaining, delta);
    range -= std::min(range, delta);

    if(!range) {
        if(current == match) {
            progress += progress == steps ? 0_ui8 : 1_ui8;
            remaining = duration;
        } else {
            progress -= progress ? 1_ui8 : 0_ui8;
        }

        range += interval;
    }

    const int left = (1 + progress / 2) - registry.size<Face>();

    for(auto i = 0; i < (left < 0 ? 0 : left); ++i) {
        spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
    }

    for(auto entity: registry.view<FaceProgress, Sprite>()) {
        registry.get<Sprite>(entity).frame = progress;
    }

    if(progress == steps) {
        next = TrainingState::IDLE;

        const auto handle = Locator::TextureCache::ref().handle("str/training/good");
        auto entity = createLastingMessage(registry, handle , 200);
        const auto &sprite = registry.get<Sprite>(entity);
        setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);

        // resets match somehow so that users must keep it to smash everything
        match = (current == FaceType::HAPPY ? FaceType::ANGRY : FaceType::HAPPY);

        for(auto i = 0; i < steps; ++i) {
            spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
        }
    } else if(!hurry && remaining < (duration / 2)) {
        const auto handle = Locator::TextureCache::ref().handle("str/training/hurry");
        auto entity = createLastingMessage(registry, handle , 200);
        const auto &sprite = registry.get<Sprite>(entity);
        setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);
    } if(!remaining) {
        next = TrainingState::IDLE;

        const auto handle = Locator::TextureCache::ref().handle("str/training/fail");
        auto entity = createLastingMessage(registry, handle , 200);
        const auto &sprite = registry.get<Sprite>(entity);
        setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);
    }

    return next;
}


TrainingSystem::TrainingSystem()
    : current{FaceType::HAPPY},
      match{FaceType::ANGRY},
      progress{0_ui8},
      range{0_ui32},
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
            enableFaceButtons(registry);
            break;
        case TrainingState::START_TRAINING:
            disableFaceButtons(registry);
            progress = 0_ui8;
            range = interval;
            remaining = duration;
            state = TrainingState::TRAINING;
            // resets match somehow so that users must keep it to smash everything
            match = (current == FaceType::HAPPY ? FaceType::ANGRY : FaceType::HAPPY);
            break;
        case TrainingState::TRAINING:
            state = training(registry, spawner, delta);
            break;
        }
    } else {
        state = TrainingState::IDLE;
        disableFaceButtons(registry);
    }
}


}
