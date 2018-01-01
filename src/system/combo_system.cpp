#include <algorithm>
#include "../component/component.hpp"
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "combo_system.h"


namespace gamee {


void ComboSystem::combo(Registry &registry, SDLTextureHandle comboHandle, SDLTextureHandle scoreHandle, int value) {
    auto comboEntity = registry.create();

    registry.assign<Renderable>(comboEntity);
    registry.assign<Movement>(comboEntity, 0.f, 0.f, -.04f);
    registry.assign<FadeAnimation>(comboEntity, 255, 0, 1000_ui32);
    registry.assign<DestroyLater>(comboEntity, 1000_ui32);
    registry.assign<Sprite>(comboEntity, comboHandle, comboHandle->width(), comboHandle->height(), comboHandle->width(), comboHandle->height());
    registry.assign<Transform>(comboEntity, logicalWidth / 2.f - comboHandle->width() / 2.f, logicalHeight / 3.f - comboHandle->height());

    auto scoreEntity = registry.create();

    registry.assign<Renderable>(scoreEntity);
    registry.assign<Movement>(scoreEntity, 0.f, 0.f, -.06f);
    registry.assign<FadeAnimation>(scoreEntity, 255, 0, 1500_ui32);
    registry.assign<DestroyLater>(scoreEntity, 1500_ui32);
    registry.assign<Sprite>(scoreEntity, scoreHandle, scoreHandle->width(), scoreHandle->height(), scoreHandle->width(), scoreHandle->height());
    registry.assign<Transform>(scoreEntity, logicalWidth / 2.f - scoreHandle->width() / 2.f, logicalHeight / 3.f);
}


ComboSystem::ComboSystem()
    : remaining{interval},
      current{0_ui8}
{
    Locator::Dispatcher::ref().connect<FaceSmashEvent>(this);
    Locator::Dispatcher::ref().connect<FaceMissEvent>(this);
}


ComboSystem::~ComboSystem() {
    Locator::Dispatcher::ref().disconnect<FaceMissEvent>(this);
    Locator::Dispatcher::ref().disconnect<FaceSmashEvent>(this);
}


void ComboSystem::receive(const FaceSmashEvent &) noexcept {
    if(current < max) {
        ++current;
        remaining = interval + current * bonus;
    }
}


void ComboSystem::receive(const FaceMissEvent &) noexcept {
    current = 0_ui8;
    remaining = interval;
}


void ComboSystem::update(Registry &registry, delta_type delta) {
    if(registry.has<PlayerScore>()) {
        auto &textureCache = Locator::TextureCache::ref();

        remaining -= std::min(delta, remaining);

        if(0 == remaining) {
            switch(current) {
            case 0:
            case 1:
                // not a combo
                break;
            case 2:
                combo(registry, textureCache.handle("combo/x2"), textureCache.handle("combo/200"), 200);
                registry.get<PlayerScore>().score += 200;
                break;
            case 3:
                combo(registry, textureCache.handle("combo/x3"), textureCache.handle("combo/300"), 300);
                registry.get<PlayerScore>().score += 300;
                break;
            case 4:
                combo(registry, textureCache.handle("combo/x4"), textureCache.handle("combo/400"), 400);
                registry.get<PlayerScore>().score += 400;
                break;
            default:
                combo(registry, textureCache.handle("combo/x5"), textureCache.handle("combo/500"), 500);
                registry.get<PlayerScore>().score += 500;
                break;
            }

            remaining = interval;
            current = 0_ui8;
        }
    }
}


}
