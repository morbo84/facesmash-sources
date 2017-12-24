#ifndef FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H


#include "../types/types.hpp"
#include "../component/component.hpp"


namespace gamee {


class FaceSpawnerSystem final {
    static constexpr delta_type interval = 1000;

    Parabola spawnPath() const;

public:
    void update(Registry &, delta_type);

private:
    delta_type elapsed{};
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H
