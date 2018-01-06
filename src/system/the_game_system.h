#ifndef FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H


#include <random>
#include "../common/constants.h"
#include "../common/types.h"


namespace gamee {


class TheGameSystem final {
    static constexpr float gravity = .0005f;
    static constexpr float bottomImpulseX = .12f;
    static constexpr float bottomImpulseY = 1.4f;
    static constexpr float sideImpulseX = .5f;
    static constexpr float sideImpulseY = .4f;

    void spawn(Registry &, float, float, float, float, FaceType, FaceModifier);

    void spawnBottom(Registry &, FaceType, FaceModifier);
    void spawnTop(Registry &, FaceType, FaceModifier);
    void spawnLeft(Registry &, FaceType, FaceModifier);
    void spawnRight(Registry &, FaceType, FaceModifier);

    FaceType next() noexcept;

public:
    TheGameSystem();

    void update(Registry &);

private:
    std::mt19937 generator;
    FaceType faces[numberOfFaces];
    UInt8 curr;
};


}


#endif // FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
