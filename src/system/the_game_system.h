#ifndef FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H


#include <random>
#include "../common/constants.h"
#include "../common/types.h"


namespace gamee {


struct FaceSmash;


class TheGameSystem final {
    static constexpr float gravity = .0005f;
    static constexpr float bottomImpulseX = .12f;
    static constexpr float bottomImpulseY = 1.4f;
    static constexpr float sideImpulseX = .5f;
    static constexpr float sideImpulseY = .4f;

    void spawn(Registry &, float, float, float, float, bool, const FaceSmash &);

    void spawnBottom(Registry &, bool, const FaceSmash &);
    void spawnTop(Registry &, bool, const FaceSmash &);
    void spawnLeft(Registry &, bool, const FaceSmash &);
    void spawnRight(Registry &, bool, const FaceSmash &);

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
