#ifndef FACE_SMASH_FACTORY_SPAWNER_H
#define FACE_SMASH_FACTORY_SPAWNER_H


#include <random>
#include "../common/types.h"
#include "../common/constants.h"
#include "../common/util.h"
#include "../component/component.hpp"


namespace gamee {


class Spawner final {
    static constexpr float gravity = .0003f;
    static constexpr float bottomImpulseX = .08f;
    static constexpr float bottomImpulseY = .95f;
    static constexpr float sideImpulseX = .5f;
    static constexpr float sideImpulseY = .35f;
    static constexpr float scoreDrift = -.1f;
    static constexpr int zValue = 170;

    void spawnFromBottom(Registry &, entity_type);
    void spawnFromTop(Registry &, entity_type);
    void spawnFromLeft(Registry &, entity_type);
    void spawnFromRight(Registry &, entity_type);
    void maybeRotate(Registry &, entity_type);

    void spawnMessage(Registry &, SDLTextureHandle, float, float);

public:
    Spawner();

    void spawnFaceBottom(Registry &, Uint16, Uint16, FaceType);
    void spawnFaceTop(Registry &, Uint16, Uint16, FaceType);
    void spawnFaceLeft(Registry &, Uint16, Uint16, FaceType);
    void spawnFaceRight(Registry &, Uint16, Uint16, FaceType);

    void spawnItemBottom(Registry &, ItemType);
    void spawnItemTop(Registry &, ItemType);
    void spawnItemLeft(Registry &, ItemType);
    void spawnItemRight(Registry &, ItemType);

    void spawnSmashScore(Registry &, Uint16, float, float);
    void spawnMissScore(Registry &, Uint16, float, float);
    void spawnBonus(Registry &, float, float);
    void spawnExplosion(Registry &, float, float);

private:
    std::mt19937 generator;
};


}


#endif // FACE_SMASH_FACTORY_SPAWNER_H
