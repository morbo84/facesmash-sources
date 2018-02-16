#include <cmath>
#include <algorithm>
#include <type_traits>
#include "../component/component.hpp"
#include "../common/util.h"
#include "../locator/locator.hpp"
#include "score_system.h"


namespace gamee {


ScoreSystem::ScoreSystem() noexcept
    : current{0}, dirty{false}
{
    Locator::Dispatcher::ref().connect<SmashEvent>(this);
}


ScoreSystem::~ScoreSystem() noexcept {
    Locator::Dispatcher::ref().disconnect<SmashEvent>(this);
}


void ScoreSystem::receive(const SmashEvent &event) noexcept {
    dirty = true;
    last = event;
}


void ScoreSystem::update(Registry &registry) {
    if(registry.has<PlayerScore>()) {
        auto &playerScore = registry.get<PlayerScore>();

        if(dirty) {
            playerScore.hitAngry += last.angry;
            playerScore.hitDisgusted += last.disgusted;
            playerScore.hitHappy += last.happy;
            playerScore.hitSurprised += last.surprised;
            playerScore.hitFearful += last.fearful;
            playerScore.hitSad += last.sad;

            if(2 == last.combo) {
                ++playerScore.combo2x;
            } else if(3 == last.combo) {
                ++playerScore.combo3x;
            } else if(4 == last.combo) {
                ++playerScore.combo4x;
            } else if(5 == last.combo) {
                ++playerScore.combo5x;
            }

            playerScore.score += (last.combo + 1) * last.smash;
            playerScore.score = (last.miss > playerScore.score) ? 0 : (playerScore.score - last.miss);
        }

        if(current < playerScore.score) {
            current += std::max((playerScore.score - current) / 3, 1);
        } else if(current > playerScore.score) {
            current -= std::max((current - playerScore.score) / 3, 1);
        }

        registry.view<PlayerScoreObserver>().each([&](auto, auto &observer) {
            auto &textureCache = Locator::TextureCache::ref();
            auto symEmptyHandle = textureCache.handle("str/ ");

            // cap the score to the limit imposed by the number of entities used to represent it
            const int cap = std::pow(10, std::extent<decltype(PlayerScoreObserver::entities)>::value);
            auto score = std::min(current, cap - 1);

            const int last = std::extent<decltype(PlayerScoreObserver::entities)>::value;
            const int offset = numOfDigits(score);

            for(auto i = offset; i < last; ++i) {
                registry.accomodate<HUD>(observer.entities[i], symEmptyHandle, symEmptyHandle->width(), symEmptyHandle->height(), symEmptyHandle->width(), symEmptyHandle->height());
            }

            for(auto i = offset; i > 0; --i) {
                auto handle = toStrHud(score % 10);
                registry.accomodate<HUD>(observer.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
                score /= 10;
            }
        });
    } else {
        current = 0;
    }

    dirty = false;
}


}
