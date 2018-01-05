#ifndef FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H


#include <random>
#include "../common/types.h"


namespace gamee {


struct SpawnRequest;


class FaceSpawnerSystem final {
    static constexpr UInt8 length = 7;

    void spawn(Registry &, const SpawnRequest &, TextureCache::resource_type, FaceType, FaceModifier);

public:
    FaceSpawnerSystem();

    void update(Registry &, delta_type);

private:
    std::mt19937 generator;
    FaceType faces[length];
    UInt8 next;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H
