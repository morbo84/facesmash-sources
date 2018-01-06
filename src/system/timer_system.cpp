#include <sstream>
#include <algorithm>
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "timer_system.h"


namespace gamee {


void TimerSystem::update(Registry &registry, GameRenderer &renderer, delta_type delta) {
    if(registry.has<LetsPlay>()) {
        assert(registry.has<GameTimer>());

        auto entity = registry.attachee<GameTimer>();
        auto &timer = registry.get<GameTimer>();

        timer.remaining -= std::min(timer.remaining, delta);

        std::stringstream label;
        label << (timer.remaining / 1000);
        const SDL_Color fg = { 255, 255, 255, 255 };
        auto font = Locator::TTFFontCache::ref().handle("ttf/constant/54");
        auto handle = Locator::TextureCache::ref().temp<TTFFontTextureLoader>(label.str().c_str(), renderer, font.get(), fg);
        registry.accomodate<HUD>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());

        if(0 == timer.remaining) {
            if(registry.has<SceneChangeRequest>()) {
                registry.destroy(registry.attachee<SceneChangeRequest>());
            }

            registry.attach<SceneChangeRequest>(registry.create(), SceneType::GAME_OVER);
        }
    }
}


}
