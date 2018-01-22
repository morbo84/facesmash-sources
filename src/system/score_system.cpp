#include <cmath>
#include <algorithm>
#include <type_traits>
#include "../component/component.hpp"
#include "../common/util.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../game/game_renderer.h"
#include "score_system.h"


namespace gamee {


void ScoreSystem::update(Registry &registry) {
    if(registry.has<LetsPlay>()) {
        auto &textureCache = Locator::TextureCache::ref();
        auto &playerScore = registry.get<PlayerScore>();
        auto symEmptyHandle = textureCache.handle("str/hud/ ");

        // cap the score to the limit imposed by the number of entities used to represent it
        const int cap = std::pow(10, std::extent<decltype(PlayerScore::entities)>::value);
        playerScore.score = std::min(playerScore.score, cap - 1);

        if(playerScore.current < playerScore.score) {
            playerScore.current += std::max((playerScore.score - playerScore.current) / 3, 1);
        } else if(playerScore.current > playerScore.score) {
            playerScore.current -= std::max((playerScore.current - playerScore.score) / 3, 1);
        }

        const int last = std::extent<decltype(PlayerScore::entities)>::value;
        auto score = playerScore.current;
        const int offset = numOfDigits(score);

        for(auto i = offset; i < last; ++i) {
            registry.accomodate<HUD>(playerScore.entities[i], symEmptyHandle, symEmptyHandle->width(), symEmptyHandle->height(), symEmptyHandle->width(), symEmptyHandle->height());
        }

        for(auto i = offset; i > 0; --i) {
            auto handle = toStrHud(score % 10);
            registry.accomodate<HUD>(playerScore.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
            score /= 10;
        }
    }
}


}
