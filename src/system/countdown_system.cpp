#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "countdown_system.h"


namespace gamee {


void CountdownSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<Countdown>()) {
        auto &countdown = registry.get<Countdown>();
        auto entity = registry.attachee<Countdown>();

        countdown.elapsed += delta;

        if(countdown.elapsed < countdown.duration) {
            const auto step = countdown.duration / 3;

            if(countdown.elapsed < step) {
                if(countdown.step < 1) {
                    auto handle = Locator::TextureCache::ref().handle("countdown/use");
                    registry.accomodate<Renderable>(entity, 0.f, 250);
                    registry.accomodate<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
                    registry.accomodate<Transform>(entity, entity, logicalWidth / 2.f - handle->width() / 2.f, logicalHeight / 2.f - handle->height() / 2.f);
                    registry.accomodate<FadeAnimation>(entity, 255, 0, step, 0_ui32, &easeInCubic);
                    ++countdown.step;
                }
            } else if(countdown.elapsed < (2*step)) {
                if(countdown.step < 2) {
                    auto handle = Locator::TextureCache::ref().handle("countdown/your");
                    registry.accomodate<Renderable>(entity, 0.f, 250);
                    registry.accomodate<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
                    registry.accomodate<Transform>(entity, entity, logicalWidth / 2.f - handle->width() / 2.f, logicalHeight / 2.f - handle->height() / 2.f);
                    registry.accomodate<FadeAnimation>(entity, 255, 0, step, 0_ui32, &easeInCubic);
                    ++countdown.step;
                }
            } else if(countdown.elapsed < (3*step)) {
                if(countdown.step < 3) {
                    auto handle = Locator::TextureCache::ref().handle("countdown/face");
                    registry.accomodate<Renderable>(entity, 0.f, 250);
                    registry.accomodate<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
                    registry.accomodate<Transform>(entity, entity, logicalWidth / 2.f - handle->width() / 2.f, logicalHeight / 2.f - handle->height() / 2.f);
                    registry.accomodate<FadeAnimation>(entity, 255, 0, step, 0_ui32, &easeInCubic);
                    ++countdown.step;
                }
            }
        } else {
            registry.destroy(entity);
        }
    }
}


}
