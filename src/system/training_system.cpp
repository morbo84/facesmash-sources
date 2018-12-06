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
      target{},
      total{},
      counter{},
      current{FaceType::HAPPY}
{
    Locator::Dispatcher::ref().sink<FaceRequest>().connect(this);
    Locator::Dispatcher::ref().sink<FaceEvent>().connect(this);
}


TrainingSystem::~TrainingSystem() {
    Locator::Dispatcher::ref().sink<FaceRequest>().disconnect(this);
    Locator::Dispatcher::ref().sink<FaceEvent>().disconnect(this);
}


void TrainingSystem::receive(const FaceRequest &event) noexcept {
    watchdog = expectation;
    remaining = duration;
    current = event.type;
    target = 0.f;
    total = 0.f;
    counter = 0_ui16;
    // starts a training session
    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING);
}


void TrainingSystem::receive(const FaceEvent &event) noexcept {
    const auto match = (event.type == current);
    target = match ? event.probability : std::max(0.f, target - .1f);
    total += match ? event.probability : 0.f;
    ++counter;
    watchdog = expectation;
}


void TrainingSystem::update(Registry &registry, Spawner &spawner, delta_type delta) {
    auto feedbackMsg = [&registry](const auto handle) {
        auto entity = createLastingMessage(registry, handle, 200);
        const auto &sprite = registry.get<Sprite>(entity);
        setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight - sprite.h) / 2);
    };

    if(!registry.empty<LetsTrain>()) {
        const auto amount = 1 + (duration - remaining) / 1000_ui32;
        const auto size = registry.size<Face>();

        if(remaining > bonus) {
            if(remaining == duration) {
                switch(current) {
                case FaceType::ANGRY:
                    feedbackMsg(Locator::TextureCache::ref().handle("str/emotion/angry"));
                    break;
                case FaceType::DISGUSTED:
                    feedbackMsg(Locator::TextureCache::ref().handle("str/emotion/disgusted"));
                    break;
                case FaceType::FEARFUL:
                    feedbackMsg(Locator::TextureCache::ref().handle("str/emotion/fearful"));
                    break;
                case FaceType::HAPPY:
                    feedbackMsg(Locator::TextureCache::ref().handle("str/emotion/happy"));
                    break;
                case FaceType::SAD:
                    feedbackMsg(Locator::TextureCache::ref().handle("str/emotion/sad"));
                    break;
                case FaceType::SURPRISED:
                    feedbackMsg(Locator::TextureCache::ref().handle("str/emotion/surprised"));
                    break;
                }
            }

            // training phase
            remaining -= std::min(remaining, delta);

            if (remaining < bonus) {
                // feedback message
                const auto result = counter ? (total / counter) : 0.f;

                if (result < .2f) {
                    feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/fail"));
                } else if (result < .4f) {
                    feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/bad"));
                } else if (result < .6f) {
                    feedbackMsg(Locator::TextureCache::ref().handle("str/feedback/basic"));
                } else if (result < .8f) {
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
            if (size < amount) {
                spawner.spawnFaceBottom(registry, 0_ui16, 0_ui16, current);
            }
        } else if(remaining) {
            remaining -= std::min(remaining, delta);
        } else {
            registry.reset<Destroyable>();
            Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::TRAINING_IS_OVER);
        }

        watchdog -= std::min(watchdog, delta);

        if(!watchdog) {
            target = std::max(0.f, target - .1f);
            watchdog = expectation;
        }

        // lerp used to obtain a smoother progress bar
        probability += factor * (target - probability) * delta;

        for(const auto entity: registry.view<VerticalProgressBar, Sprite>()) {
            auto &sprite = registry.get<Sprite>(entity);
            sprite.frame = sprite.frames * probability;
        }
    }
}


}
