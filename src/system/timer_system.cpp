#include <sstream>
#include <algorithm>
#include <SDL_pixels.h>
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "timer_system.h"


namespace gamee {


void TimerSystem::update(Registry &registry, GameRenderer &renderer, delta_type delta) {
    if(registry.has<LetsPlay>()) {
        auto entity = registry.attachee<GameTimer>();
        auto &timer = registry.get<GameTimer>();

        timer.remaining -= std::min(timer.remaining, delta);

        std::stringstream label;
        label << (timer.remaining / 1000);
        const SDL_Color fg = { 255, 255, 255, 255 };
        auto font = Locator::TTFFontCache::ref().handle("font/debug/small");
        auto handle = Locator::TextureCache::ref().temp<TTFFontTextureLoader>(label.str().c_str(), renderer, font.get(), fg);
        registry.accomodate<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());

        if(0 == timer.remaining) {
            Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::GAME_OVER);
        }
    }
}


}
