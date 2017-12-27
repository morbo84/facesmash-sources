#include <sstream>
#include <SDL_rect.h>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "face_smash_system.h"


namespace gamee {


FaceSmashSystem::FaceSmashSystem()
    : type{SmashType::HAPPY},
      dirty{false}
{
    Locator::Dispatcher::ref().connect<FaceSmashEvent>(this);
}


FaceSmashSystem::~FaceSmashSystem() {
    Locator::Dispatcher::ref().disconnect<FaceSmashEvent>(this);
}


void FaceSmashSystem::receive(const FaceSmashEvent &event) noexcept {
    type = event.type;
    dirty = true;
}


void FaceSmashSystem::update(Registry &registry, delta_type delta) {
    auto view = registry.view<FaceSmash, Transform, Movement, BoundingBox>();
    const SDL_Rect screen = logicalScreen;

    view.each([&registry, delta, screen, this](auto entity, const auto &smash, const auto &transform, const auto &movement, const auto &box) {
        const auto area = transform * box;

        if(movement.velY > 0 && !SDL_HasIntersection(&screen, &area)) {
            // TODO miss

            Locator::Dispatcher::ref().enqueue<FaceMissEvent>();
        } else if(dirty && smash.type == type) {
            //auto &ttfFontCache = Locator::TTFFontCache::ref();
            //auto &ttfFontTextureCache = Locator::TTFFontTextureCache::ref();
            auto &textureCache = Locator::TextureCache::ref();

            auto explosion = registry.create();
            registry.assign<Sprite>(explosion, textureCache.handle("game/explosion"), 192, 192, 192, 192, 0, 0, 20_ui8, 5_ui8);
            registry.assign<Renderable>(explosion, 0.f, 1.f, 255);
            registry.assign<Transform>(explosion, area.w / 2.f - 96, area.h / 2.f - 96);
            registry.assign<SpriteAnimation>(explosion, 1000_ui32, 0_ui32, false);
            registry.assign<DestroyLater>(explosion, 1000_ui32);

            //std::stringstream label;
            //time << "-" << smash.miss;

            //auto font = Locator::TTFFontCache::ref().handle("ttf/constant/36");
            //auto handle = Locator::TTFFontTextureCache::ref().temp<TTFFontTextureLoader>(label.str().c_str(), renderer, font.get(), fg);

            // TODO match

            registry.destroy(entity);
        }
    });

    dirty = false;
}


}
