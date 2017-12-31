#include <sstream>
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../game/game_renderer.h"
#include "score_system.h"


namespace gamee {


ScoreSystem::ScoreSystem()
    : elapsed{0_ui8},
      current{0_ui16},
      score{0_ui16}
{
    Locator::Dispatcher::ref().connect<ScoreEvent>(this);
}


ScoreSystem::~ScoreSystem() {
    Locator::Dispatcher::ref().disconnect<ScoreEvent>(this);
}


void ScoreSystem::receive(const ScoreEvent &event) noexcept {
    if(event.score < 0) {
        score += (-event.score > score) ? -score : event.score;
    } else {
        score += event.score;
    }
}


void ScoreSystem::update(Registry &registry, GameRenderer &renderer, delta_type delta) {
    elapsed += delta;

    if(elapsed > interval) {
        elapsed = 0_ui8;

        if(current < score) {
            current += std::max((score - current) / 2, 1);
        } else if(current > score) {
            current -= std::max((current - score) / 2, 1);
        }
    }

    if(registry.has<HUDScore>()) {
        auto entity = registry.attachee<HUDScore>();
        std::stringstream label;
        label << current;
        const SDL_Color fg = { 255, 255, 255, 255 };
        auto font = Locator::TTFFontCache::ref().handle("ttf/constant/54");
        auto handle = Locator::TextureCache::ref().temp<TTFFontTextureLoader>(label.str().c_str(), renderer, font.get(), fg);
        registry.accomodate<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    }
}


}
