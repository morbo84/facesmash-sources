#ifndef FACE_SMASH_FACTORY_FACTORY_H
#define FACE_SMASH_FACTORY_FACTORY_H


#include <random>
#include "../common/types.h"
#include "../common/constants.h"
#include "../component/component.hpp"


namespace gamee {


class Factory final {
    static constexpr float gravity = .0005f;
    static constexpr float bottomImpulseX = .12f;
    static constexpr float bottomImpulseY = 1.4f;
    static constexpr float sideImpulseX = .5f;
    static constexpr float sideImpulseY = .4f;
    static constexpr float scoreDrift = -.06f;

    SDLTextureHandle toHandle(FaceType);
    SDLTextureHandle toHandle(ItemType);

    int toZ(FaceType);
    int toZ(ItemType);

    ItemType nextItem() noexcept;
    FaceType nextFace() noexcept;

    void addScore(Registry &, SDLTextureHandle, float, float);

    template<typename Component>
    void spawnFaceOrItem(Registry &registry, float x, float y, float impulseX, float impulseY, const Component &component) {
        auto handle = toHandle(component.type);
        auto entity = registry.create();

        registry.assign<Destroyable>(entity);
        registry.assign<Renderable>(entity, 0.f, toZ(component.type));
        registry.assign<Transform>(entity, entity, x, y);
        registry.assign<Movement>(entity, gravity, impulseX, impulseY);
        registry.assign<Sprite>(entity, handle, handle->width(), handle->height(), handle->width(), handle->height());
        registry.assign<BoundingBox>(entity, handle->width(), handle->height());
        registry.assign<Component>(entity, component);

        delta_type duration = 10000_ui32 + generator() % 10000_ui32;

        if(generator() > (generator.max() / 2)) {
            registry.assign<RotationAnimation>(entity, 0.f, 360.f, duration);
        } else {
            registry.assign<RotationAnimation>(entity, 360.f, 0.f, duration);
        }
    }

    template<typename Component>
    void spawnFaceOrItemBottom(Registry &registry, const Component &component) {
        const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (toHandle(component.type)->width() / 2);
        const int y = logicalHeight - 1;

        const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
        const float impulseY = -1.f * bottomImpulseY * (generator() / (4.f * generator.max()) + .65f);

        spawnFaceOrItem(registry, x, y, impulseX, impulseY, component);
    }

    template<typename Component>
    void spawnFaceOrItemTop(Registry &registry, const Component &component) {
        auto handle = toHandle(component.type);

        const int x = (generator() % (logicalWidth / 4)) + (3 * logicalWidth / 8) - (handle->width() / 2);
        const int y = -handle->height() + 1;

        const float impulseX = bottomImpulseX * (2.f * generator() / generator.max() - 1.f);
        const float impulseY = gravity / 1000;

        spawnFaceOrItem(registry, x, y, impulseX, impulseY, component);
    }

    template<typename Component>
    void spawnFaceOrItemLeft(Registry &registry, const Component &component) {
        const int x = -toHandle(component.type)->width() + 1;
        const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

        const float impulseX = sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
        const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

        spawnFaceOrItem(registry, x, y, impulseX, impulseY, component);
    }

    template<typename Component>
    void spawnFaceOrItemRight(Registry &registry, const Component &component) {
        const int x = logicalWidth - 1;
        const int y = (generator() % (logicalHeight / 4)) + (logicalHeight / 4);

        const float impulseX = -sideImpulseX * (generator() / (4.f * generator.max()) + .65f);
        const float impulseY = -1.f * sideImpulseY * (generator() / (4.f * generator.max()) + .65f);

        spawnFaceOrItem(registry, x, y, impulseX, impulseY, component);
    }

public:
    Factory();

    void spawnFaceBottom(Registry &, UInt8, UInt8);
    void spawnFaceTop(Registry &, UInt8, UInt8);
    void spawnFaceLeft(Registry &, UInt8, UInt8);
    void spawnFaceRight(Registry &, UInt8, UInt8);

    void spawnItemBottom(Registry &);
    void spawnItemTop(Registry &);
    void spawnItemLeft(Registry &);
    void spawnItemRight(Registry &);

    void spawnSmashScore(Registry &, UInt8, float, float);
    void spawnMissScore(Registry &, UInt8, float, float);
    void spawnExplosion(Registry &, float, float);

private:
    std::mt19937 generator;
    ItemType items[numberOfItems];
    FaceType faces[numberOfFaces];
    UInt8 item;
    UInt8 face;
};


}


#endif // FACE_SMASH_FACTORY_FACTORY_H
