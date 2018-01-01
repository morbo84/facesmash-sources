#include <algorithm>
#include <sstream>
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../game/game_renderer.h"
#include "score_system.h"


namespace gamee {


void ScoreSystem::update(Registry &registry, GameRenderer &renderer, delta_type delta) {
    if(registry.has<PlayerScore>()) {
        auto entity = registry.attachee<PlayerScore>();
        auto &score = registry.get<PlayerScore>();

        elapsed += delta;

        if(elapsed > interval) {
            elapsed = 0_ui8;

            if(current < score.score) {
                current += std::max((score.score - current) / 2, 1);
            } else if(current > score.score) {
                current -= std::max((current - score.score) / 2, 1);
            }
        }

        std::stringstream label;
        label << current;
        const SDL_Color fg = { 255, 255, 255, 255 };
        auto font = Locator::TTFFontCache::ref().handle("ttf/constant/54");
        auto handle = Locator::TextureCache::ref().temp<TTFFontTextureLoader>(label.str().c_str(), renderer, font.get(), fg);
        registry.accomodate<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    }
}


}
