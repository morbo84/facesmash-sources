#include <cassert>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "the_game_system.h"


namespace gamee {


static auto toHandle(const FaceSmash &smash) {
    // unfortunately we have not (yet) modifiers for all the faces
    // therefore modifiers come before the face type here

    switch(smash.modifier) {
    case FaceModifier::NONE:
        switch(smash.type) {
        case FaceType::ANGRY:
            return Locator::TextureCache::ref().handle("emoji/angry");
        case FaceType::DISGUSTED:
            return Locator::TextureCache::ref().handle("emoji/disgusted");
        case FaceType::FEARFUL:
            return Locator::TextureCache::ref().handle("emoji/fearful");
        case FaceType::HAPPY:
            return Locator::TextureCache::ref().handle("emoji/happy");
        case FaceType::SAD:
            return Locator::TextureCache::ref().handle("emoji/sad");
        case FaceType::SURPRISED:
            return Locator::TextureCache::ref().handle("emoji/surprised");
        default:
            assert(false); // suppress warnings
        }
    case FaceModifier::SLOW_DOWN:
        return Locator::TextureCache::ref().handle("emoji/star");
    case FaceModifier::SPEED_UP:
        return Locator::TextureCache::ref().handle("emoji/pouting");
    default:
        assert(false); // suppress warnings
    }
}


void TheGameSystem::spawn(Registry &registry, float x, float y, float impulseX, float impulseY, bool rotation, const FaceSmash &smash) {
    auto handle = toHandle(smash);
    auto entity = registry.create<Renderable>();

    registry.assign<Transform>(entity, entity, x, y);
    registry.assign<Movement>(entity, gravity, impulseX, impulseY);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<BoundingBox>(entity, handle->width(), handle->height());
    registry.assign<FaceSmash>(entity, smash);

    if(rotation) {
        delta_type duration = 10000_ui32 + generator() % 10000_ui32;

        if(generator() > (generator.max() / 2)) {
            registry.assign<RotationAnimation>(entity, 0.f, 360.f, duration);
        } else {
            registry.assign<RotationAnimation>(entity, 360.f, 0.f, duration);
        }
    }
}


void TheGameSystem::spawnBottom(Registry &registry, bool rotation, const FaceSmash &smash) {
    const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (toHandle(smash)->width() / 2);
    const int y = logicalHeight;

    const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
    const float impulseY = -1.f * bottomImpulseY * (generator() / (4.f * generator.max()) + .65f);

    spawn(registry, x, y, impulseX, impulseY, rotation, smash);
}


void TheGameSystem::spawnTop(Registry &registry, bool rotation, const FaceSmash &smash) {
    auto handle = toHandle(smash);

    const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (handle->width() / 2);
    const int y = -handle->height() + 1;

    const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
    const float impulseY = gravity / 1000;

    spawn(registry, x, y, impulseX, impulseY, rotation, smash);
}


void TheGameSystem::spawnLeft(Registry &registry, bool rotation, const FaceSmash &smash) {
    const int x = -toHandle(smash)->width();
    const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

    const float impulseX = sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
    const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

    spawn(registry, x, y, impulseX, impulseY, rotation, smash);
}


void TheGameSystem::spawnRight(Registry &registry, bool rotation, const FaceSmash &smash) {
    const int x = logicalWidth;
    const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

    const float impulseX = -sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
    const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

    spawn(registry, x, y, impulseX, impulseY, rotation, smash);
}


FaceType TheGameSystem::next() noexcept {
    if(curr == numberOfFaces) {
        std::shuffle(faces, faces+numberOfFaces, generator);
        curr = 0_ui8;
    }

    return faces[curr++];
}


TheGameSystem::TheGameSystem()
    : generator{std::random_device{}()},
      faces{
          FaceType::ANGRY,
          FaceType::DISGUSTED,
          FaceType::FEARFUL,
          FaceType::HAPPY,
          FaceType::SAD,
          FaceType::SURPRISED
      },
      curr{numberOfFaces}
{}


void TheGameSystem::update(Registry &registry) {
    if(registry.has<LetsPlay>()) {
        assert(registry.has<PlayerScore>());
        assert(registry.has<GameTimer>());

        const auto &remaining = registry.get<GameTimer>().remaining;
        auto &play = registry.get<LetsPlay>();

        if(remaining > 30000 && registry.size<FaceSmash>() < 1) {
            spawnBottom(registry, false, FaceSmash{50_ui8, 10_ui8, next()});
        } else if(remaining <= 30000 && registry.size<FaceSmash>() < 2) {
            spawnBottom(registry, true, FaceSmash{100_ui8, 20_ui8, next()});
        }

        if(remaining < 55000 && !play.remaining55000) {
            spawnLeft(registry, false, FaceSmash{50_ui8, 10_ui8, next()});
            spawnLeft(registry, true, FaceSmash{50_ui8, 10_ui8, next()});
            spawnRight(registry, false, FaceSmash{50_ui8, 10_ui8, next()});
            spawnRight(registry, true, FaceSmash{50_ui8, 10_ui8, next()});
            spawnTop(registry, false, FaceSmash{0_ui8, 0_ui8, FaceType::HAPPY, FaceModifier::SLOW_DOWN});
            play.remaining55000 = true;
        }

        if(remaining < 50000 && !play.remaining50000) {
            spawnBottom(registry, true, FaceSmash{200_ui8, 50_ui8, next()});
            spawnBottom(registry, true, FaceSmash{200_ui8, 50_ui8, next()});
            spawnTop(registry, false, FaceSmash{0_ui8, 0_ui8, FaceType::ANGRY, FaceModifier::SPEED_UP});
            play.remaining50000 = true;
        }

        if(remaining < 40000 && !play.remaining40000) {
            spawnLeft(registry, false, FaceSmash{100_ui8, 20_ui8, next()});
            spawnRight(registry, false, FaceSmash{100_ui8, 20_ui8, next()});
            spawnTop(registry, true, FaceSmash{200_ui8, 50_ui8, next()});
            spawnBottom(registry, false, FaceSmash{0_ui8, 0_ui8, FaceType::HAPPY, FaceModifier::SLOW_DOWN});
            play.remaining40000 = true;
        }

        if(remaining < 30000 && !play.remaining30000) {
            spawnTop(registry, false, FaceSmash{0_ui8, 0_ui8, FaceType::ANGRY, FaceModifier::SPEED_UP});
            spawnTop(registry, true, FaceSmash{0_ui8, 0_ui8, FaceType::HAPPY, FaceModifier::SLOW_DOWN});
            play.remaining30000 = true;
        }

        if(remaining < 15000 && !play.remaining15000) {
            spawnLeft(registry, true, FaceSmash{100_ui8, 20_ui8, next()});
            spawnRight(registry, true, FaceSmash{100_ui8, 20_ui8, next()});
            spawnTop(registry, false, FaceSmash{200_ui8, 50_ui8, next()});
            spawnBottom(registry, false, FaceSmash{0_ui8, 0_ui8, FaceType::HAPPY, FaceModifier::SLOW_DOWN});
            play.remaining15000 = true;
        }

        if(remaining < 7500) {
            if(!play.remaining7500) {
                spawnBottom(registry, false, FaceSmash{0_ui8, 0_ui8, FaceType::HAPPY, FaceModifier::SLOW_DOWN});
                play.remaining7500 = true;
            }

            if(registry.size<FaceSmash>() < 10) {
                spawnBottom(registry, false, FaceSmash{250_ui8, 100_ui8, next()});
            }
        }

        if(registry.get<PlayerScore>().score > play.nextScoreStep) {
            if(play.nextScoreStep > 5000) {
                spawnLeft(registry, true, FaceSmash{200_ui8, 100_ui8, next()});
                spawnRight(registry, true, FaceSmash{200_ui8, 100_ui8, next()});
            } else {
                spawnLeft(registry, false, FaceSmash{100_ui8, 100_ui8, next()});
                spawnRight(registry, false, FaceSmash{100_ui8, 100_ui8, next()});
            }

            play.nextScoreStep += 2500;
        }
    }
}


}
