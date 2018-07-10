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
    : watchdog{},
      remaining{},
      probability{},
      total{},
      counter{},
      current{FaceType::HAPPY},
      steps{}
{
    Locator::Dispatcher::ref().connect<FaceRequest>(this);
    Locator::Dispatcher::ref().connect<FaceEvent>(this);
}


TrainingSystem::~TrainingSystem() {
    Locator::Dispatcher::ref().disconnect<FaceRequest>(this);
    Locator::Dispatcher::ref().disconnect<FaceEvent>(this);
}


void TrainingSystem::receive(const FaceRequest &event) noexcept {
    watchdog = expectation;
    remaining = interval;
    current = event.type;
    probability = 0.f;
    total = 0.f;
    counter = 0_ui16;
    steps = length;
    // starts a training session
    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING);
}


void TrainingSystem::receive(const FaceEvent &event) noexcept {
    const auto match = event.type == current;
    probability = match ? event.probability : std::max(0.f, probability - .1f);
    total += match ? event.probability : 0.f;
    ++counter;
    watchdog = expectation;
}


void TrainingSystem::update(Registry &registry, Spawner &spawner, delta_type delta) {

    if(registry.has<LetsTrain>()) {
        watchdog -= std::min(watchdog, delta);

        if(!watchdog) {
            probability = std::max(0.f, probability - .1f);
            watchdog = expectation;
        }

        if(steps) {
            // training phase

            remaining -= std::min(remaining, delta);
            const auto amount = ((length - steps) * interval) / 1000_ui32;
            const auto size = registry.size<Face>();

            // ensure there is at least a face on screen
            if(!size) {
                spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
            }

            if(!remaining) {
                --steps;

                if(steps) {
                    remaining = interval;
                } else {
                    // bonus time to allow user to smash everything
                    remaining = bonus * interval;

                    // feedback message
                    const auto result = counter ? (total / counter) : 0.f;

                    auto feedbackMsg = [&registry, this](const auto handle) {
                        auto entity = createLastingMessage(registry, handle , 200);
                        const auto &sprite = registry.get<Sprite>(entity);
                        setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);
                    };

                    if(result < .2f) {
                        feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/fail"));
                    } else if(result < .4f) {
                        feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/bad"));
                    } else if(result < .6f) {
                        feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/basic"));
                    } else if(result < .8f) {
                        feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/good"));
                    } else {
                        feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/wow"));
                        // spawns extra faces to celebrate the great result
                        spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
                        spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
                        spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
                    }
                }

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

        registry.view<VerticalProgressBar, Sprite>().each([this](auto entity, auto &, auto &sprite) {
            sprite.frame = (sprite.frames - 1) * probability;
        });
    }
}


}
