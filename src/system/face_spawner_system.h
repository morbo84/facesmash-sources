#ifndef FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H


#include "../types/types.hpp"
#include "../component/component.hpp"


namespace gamee {


struct FaceSpawnerSystem final {
    void update(Registry &, delta_type);


private:
    Parabola spawnPath() const;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SPAWNER_SYSTEM_H
