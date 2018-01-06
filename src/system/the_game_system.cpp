#include <cassert>
#include "../common/constants.h"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "the_game_system.h"


namespace gamee {


static auto toHandle(FaceType type, FaceModifier modifier) {
    // unfortunately we have not (yet) modifiers for all the faces
    // therefore modifiers come before the face type here

    switch(modifier) {
    case FaceModifier::NONE:
        switch(type) {
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


void TheGameSystem::spawn(Registry &registry, float x, float y, float impulseX, float impulseY, FaceType type, FaceModifier modifier) {
    auto handle = toHandle(type, modifier);
    auto entity = registry.create<Renderable>();
    registry.assign<Transform>(entity, x, y);
    registry.assign<Movement>(entity, gravity, impulseX, impulseY);
    registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
    registry.assign<BoundingBox>(entity, handle->width(), handle->height());
    registry.assign<FaceSmash>(entity, type, modifier);
}


void TheGameSystem::spawnBottom(Registry &registry, FaceType type, FaceModifier modifier) {
    const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (toHandle(type, modifier)->width() / 2);
    const int y = logicalHeight;

    const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
    const float impulseY = -1.f * bottomImpulseY * (generator() / (4.f * generator.max()) + .65f);

    spawn(registry, x, y, impulseX, impulseY, type, modifier);
}


void TheGameSystem::spawnTop(Registry &registry, FaceType type, FaceModifier modifier) {
    auto handle = toHandle(type, modifier);

    const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (handle->width() / 2);
    const int y = -handle->height() + 1;

    const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
    const float impulseY = gravity / 1000;

    spawn(registry, x, y, impulseX, impulseY, type, modifier);
}


void TheGameSystem::spawnLeft(Registry &registry, FaceType type, FaceModifier modifier) {
    const int x = -toHandle(type, modifier)->width();
    const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

    const float impulseX = sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
    const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

    spawn(registry, x, y, impulseX, impulseY, type, modifier);
}


void TheGameSystem::spawnRight(Registry &registry, FaceType type, FaceModifier modifier) {
    const int x = logicalWidth;
    const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

    const float impulseX = -sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
    const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

    spawn(registry, x, y, impulseX, impulseY, type, modifier);
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
      curr{numberOfFaces},
      remaining55000{false},
      remaining50000{false},
      remaining40000{false},
      remaining30000{false},
      remaining15000{false},
      remaining5000{false},
      nextScoreStep{2500}
{}


void TheGameSystem::update(Registry &registry) {
    if(registry.has<LetsPlay>()) {
        assert(registry.has<PlayerScore>());
        assert(registry.has<GameTimer>());

        const auto &remaining = registry.get<GameTimer>().remaining;

        if((remaining > 30000 && registry.size<FaceSmash>() < 1)
                || (remaining <= 30000 && registry.size<FaceSmash>() < 2)) {
            spawnBottom(registry, next(), FaceModifier::NONE);
        }

        if(remaining < 55000 && !remaining55000) {
            spawnLeft(registry, next(), FaceModifier::NONE);
            spawnLeft(registry, next(), FaceModifier::NONE);
            spawnRight(registry, next(), FaceModifier::NONE);
            spawnRight(registry, next(), FaceModifier::NONE);
            spawnTop(registry, FaceType::HAPPY, FaceModifier::SLOW_DOWN);
            remaining55000 = true;
        }

        if(remaining < 50000 && !remaining50000) {
            spawnBottom(registry, next(), FaceModifier::NONE);
            spawnBottom(registry, next(), FaceModifier::NONE);
            spawnTop(registry, FaceType::ANGRY, FaceModifier::SPEED_UP);
            remaining50000 = true;
        }

        if(remaining < 40000 && !remaining40000) {
            spawnLeft(registry, next(), FaceModifier::NONE);
            spawnRight(registry, next(), FaceModifier::NONE);
            spawnTop(registry, next(), FaceModifier::NONE);
            spawnBottom(registry, FaceType::HAPPY, FaceModifier::SLOW_DOWN);
            remaining40000 = true;
        }

        if(remaining < 30000 && !remaining30000) {
            spawnTop(registry, FaceType::ANGRY, FaceModifier::SPEED_UP);
            spawnTop(registry, FaceType::HAPPY, FaceModifier::SLOW_DOWN);
            remaining30000 = true;
        }

        if(remaining < 15000 && !remaining15000) {
            spawnLeft(registry, next(), FaceModifier::NONE);
            spawnRight(registry, next(), FaceModifier::NONE);
            spawnTop(registry, next(), FaceModifier::NONE);
            spawnBottom(registry, FaceType::HAPPY, FaceModifier::SLOW_DOWN);
            remaining15000 = true;
        }

        if(remaining < 5000) {
            if(!remaining5000) {
                spawnBottom(registry, FaceType::HAPPY, FaceModifier::SLOW_DOWN);
                remaining5000 = true;
            }

            if(registry.size<FaceSmash>() < 10) {
                spawnBottom(registry, next(), FaceModifier::NONE);
            }
        }

        const auto &score = registry.get<PlayerScore>().score;

        if(false && score > nextScoreStep) {
            spawnLeft(registry, next(), FaceModifier::NONE);
            spawnRight(registry, next(), FaceModifier::NONE);
            nextScoreStep += 2500;
        }
    }
}


}
