#include <SDL_rect.h>
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"
#include "../factory/common.h"
#include "../factory/play_factory.h"
#include "../factory/spawner.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "face_smash_system.h"


namespace gamee {


FaceSmashSystem::FaceSmashSystem()
    : type{FaceType::HAPPY},
      dirty{false},
      smashAll{false},
      richMan{false},
      armageddon{false}
{
    Locator::Dispatcher::ref().sink<FaceEvent>().connect(this);
    Locator::Dispatcher::ref().sink<BonusEvent>().connect(this);
    Locator::Dispatcher::ref().sink<ArmageddonEvent>().connect(this);
}


FaceSmashSystem::~FaceSmashSystem() {
    Locator::Dispatcher::ref().sink<ArmageddonEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<BonusEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<FaceEvent>().disconnect(this);
}


void FaceSmashSystem::receive(const FaceEvent &event) noexcept {
    if(event.probability >= probabilityThreshold) {
        type = event.type;
        dirty = true;
    }
}


void FaceSmashSystem::receive(const BonusEvent &event) noexcept {
    smashAll = (event.type == BonusEvent::Type::SMASH_ALL);
    richMan = (event.type == BonusEvent::Type::I_AM_RICH);
}


void FaceSmashSystem::receive(const ArmageddonEvent &) noexcept {
    armageddon = true;
}


void FaceSmashSystem::update(Registry &registry, Spawner &spawner) {
    auto &textureCache = Locator::TextureCache::ref();

    auto view = registry.view<Face, Transform, BoundingBox>();
    SmashEvent event{};

    SDL_Rect smashArea = logicalScreen;

    if(!registry.empty<LetsPlay>()) {
        smashArea = playArea;
    } else if(!registry.empty<LetsTrain>()) {
        smashArea = trainingArea;
    }

    view.each([&, this](auto entity, auto &face, const auto &transform, const auto &box) {
        const auto area = transformToPosition(registry, entity, transform) * box;

        const auto x = area.x + area.w / 2;
        const auto y = area.y + area.h / 2;

        if(registry.has<Destroyable>(entity)) {
            if(SDL_HasIntersection(&smashArea, &area)) {
                if(smashAll || (dirty && face.type == type)) {
                    switch(face.type) {
                    case FaceType::ANGRY:
                        ++event.angry;
                        break;
                    case FaceType::DISGUSTED:
                        ++event.disgusted;
                        break;
                    case FaceType::HAPPY:
                        ++event.happy;
                        break;
                    case FaceType::SURPRISED:
                        ++event.surprised;
                        break;
                    case FaceType::FEARFUL:
                        ++event.fearful;
                        break;
                    case FaceType::SAD:
                        ++event.sad;
                        break;
                    };

                    auto smash = face.smash;

                    if(smashAll) {
                        smash /= 10_ui8;
                    } else if(face.bonus) {
                        smash *= 10_ui8;
                    }

                    spawner.spawnExplosion(registry, x, y);
                    spawner.spawnSmashScore(registry, smash, x, y);

                    event.smash += smash;
                    ++event.combo;

                    registry.destroy(entity);
                } else if(richMan && !face.bonus) {
                    spawner.spawnBonus(registry, x, y);
                    face.bonus = true;
                }
            } else if(!SDL_HasIntersection(&logicalScreen, &area)) {
                spawner.spawnMissScore(registry, face.miss, x, y);
                event.miss += face.miss;
                registry.destroy(entity);
            }
        } else if(armageddon) {
            spawner.spawnExplosion(registry, x, y);
            registry.destroy(entity);
        } else if(!SDL_HasIntersection(&logicalScreen, &area)) {
            registry.destroy(entity);
        }
    });

    armageddon = false;
    smashAll = false;
    richMan = false;
    dirty = false;

    // check rewards
    auto reward = [&](auto handle, auto shake) {
        auto &camera = *registry.raw<Camera>();
        camera.remaining = shakeDuration;
        camera.shake = shake;

        if(!registry.empty<LetsPlay>()) {
            for(auto entity: registry.view<FaceSmashMessage>()) {
                registry.destroy(entity);
            }

            auto entity = createLightningMessage(registry, handle, 200);
            registry.assign<FaceSmashMessage>(entity);
            const auto &sprite = registry.get<Sprite>(entity);
            setPos(registry, entity, (logicalWidth - sprite.w) / 2, (logicalHeight / 4 - sprite.h) / 2);
        }
    };

    if(event.combo > 5) {
        // max combo allowed
        event.combo = 5;
    }

    auto &haptic = Locator::Haptic::ref();

    if(1 == event.combo && registry.empty<Face>()) {
        // no more faces to smash
        reward(textureCache.handle("str/reward/perfect"), 8_ui8);
        haptic.rumble(RumbleEffect::SUPER_HARD);
    } else if(2 == event.combo) {
        // 2x combo
        reward(textureCache.handle("str/reward/x2"), 2_ui8);
        haptic.rumble(RumbleEffect::SUPER_SOFT);
    } else if(3 == event.combo) {
        // 3x combo
        reward(textureCache.handle("str/reward/x3"), 4_ui8);
        haptic.rumble(RumbleEffect::SOFT);
    } else if(4 == event.combo) {
        // 4x combo
        reward(textureCache.handle("str/reward/x4"), 6_ui8);
        haptic.rumble(RumbleEffect::NORMAL);
    } else if(5 == event.combo) {
        // 5x combo
        reward(textureCache.handle("str/reward/x5"), 8_ui8);
        haptic.rumble(RumbleEffect::HARD);
    }

    Locator::Dispatcher::ref().enqueue<SmashEvent>(event);
}


}
