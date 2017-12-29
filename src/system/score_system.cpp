#include <sstream>
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../game/game_renderer.h"
#include "score_system.h"


namespace gamee {


ScoreSystem::ScoreSystem(): score{0} {
    Locator::Dispatcher::ref().connect<ScoreEvent>(this);
}


ScoreSystem::~ScoreSystem() {
    Locator::Dispatcher::ref().disconnect<ScoreEvent>(this);
}


void ScoreSystem::receive(const ScoreEvent &event) noexcept {
    score += event.score;
}


void ScoreSystem::update(Registry &registry, GameRenderer &renderer) {
    auto entity = registry.attachee<HUDScore>();
    std::stringstream label;
    label << score;
    const SDL_Color fg = { 255, 255, 255, 255 };
    auto font = Locator::TTFFontCache::ref().handle("ttf/constant/90");
    auto handle = Locator::TextureCache::ref().temp<TTFFontTextureLoader>(label.str().c_str(), renderer, font.get(), fg);
    registry.accomodate<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
}


}
