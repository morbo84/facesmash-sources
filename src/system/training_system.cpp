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


TrainingSystem::TrainingSystem()
    : current{FaceType::HAPPY},
      match{FaceType::ANGRY},
      progress{0_ui8},
      range{0_ui32},
      remaining{0_ui32},
      userDidIt{false}
{
    Locator::Dispatcher::ref().connect<FaceRequest>(this);
    Locator::Dispatcher::ref().connect<FaceEvent>(this);
}


TrainingSystem::~TrainingSystem() {
    Locator::Dispatcher::ref().disconnect<FaceRequest>(this);
    Locator::Dispatcher::ref().disconnect<FaceEvent>(this);
}


void TrainingSystem::receive(const FaceRequest &event) noexcept {
    current = event.type;
    remaining = duration;
    progress = 0_ui8;
    userDidIt = false;
    // resets the match somehow
    match = (current == FaceType::HAPPY ? FaceType::ANGRY : FaceType::HAPPY);
    // starts a training session
    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING);
}


void TrainingSystem::receive(const FaceEvent &event) noexcept {
    if(event.probability >= probabilityTreshold) {
        match = event.type;
    }
}


void TrainingSystem::update(Registry &registry, Spawner &spawner, delta_type delta) {
    if(registry.has<LetsTrain>()) {
        if(userDidIt) {
            remaining -= std::min(remaining, delta);

            if(!remaining || !registry.size<Face>()) {
                Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING_SELECT);
            }
        } else {
            const bool hurry = remaining < (duration / 2);

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

                const int left = (1 + progress / 2) - registry.size<Face>();

                for(auto i = 0; i < (left < 0 ? 0 : left); ++i) {
                    spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
                }

                for(auto entity: registry.view<FaceProgress, Sprite>()) {
                    registry.get<Sprite>(entity).frame = progress;
                }
            }

            if(progress == steps) {
                const auto handle = Locator::TextureCache::ref().handle("str/training/good");
                auto entity = createLastingMessage(registry, handle , 200);
                const auto &sprite = registry.get<Sprite>(entity);
                setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);

                for(auto i = 0; i < steps; ++i) {
                    spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
                }

                remaining = bonus;
                userDidIt = true;
            } else if(!hurry && remaining < (duration / 2)) {
                const auto handle = Locator::TextureCache::ref().handle("str/training/hurry");
                auto entity = createLastingMessage(registry, handle , 200);
                const auto &sprite = registry.get<Sprite>(entity);
                setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);
            } if(!remaining) {
                const auto handle = Locator::TextureCache::ref().handle("str/training/fail");
                auto entity = createLastingMessage(registry, handle , 200);
                const auto &sprite = registry.get<Sprite>(entity);
                setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);

                Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING_SELECT);
            }
        }
    }
}


}
