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
    : remaining{},
      probability{},
      current{FaceType::HAPPY},
      counter{}
{
    Locator::Dispatcher::ref().connect<FaceRequest>(this);
    Locator::Dispatcher::ref().connect<FaceEvent>(this);
}


TrainingSystem::~TrainingSystem() {
    Locator::Dispatcher::ref().disconnect<FaceRequest>(this);
    Locator::Dispatcher::ref().disconnect<FaceEvent>(this);
}


void TrainingSystem::receive(const FaceRequest &event) noexcept {
    remaining = interval;
    current = event.type;
    probability = 0.f;
    counter = steps;
    // starts a training session
    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING);
}


void TrainingSystem::receive(const FaceEvent &event) noexcept {
    probability = (event.type == current) ? event.probability : 0.f;
}


void TrainingSystem::update(Registry &registry, Spawner &spawner, delta_type delta) {
    if(registry.has<LetsTrain>()) {
        if(counter) {
            // training phase

            remaining -= std::min(remaining, delta);
            const auto amount = ((steps - counter) * interval) / 1000_ui32;
            const auto size = registry.size<Face>();

            // ensure there is at least a face on screen
            if(!size) {
                spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
            }

            if(!remaining) {
                // TODO plot report line

                --counter;
                // bonus time to allow user to smash everything
                remaining = counter ? interval : (bonus * interval);

                // spawn extra faces whether required
                if(size < amount) {
                    spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
                }
            }
        } else if(remaining && registry.size<Face>()) {
            remaining -= std::min(remaining, delta);
        } else {
            Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING_SELECT);
        }
    }
}


}
