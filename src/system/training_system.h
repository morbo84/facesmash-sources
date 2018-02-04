#ifndef FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
#define FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class Spawner;


struct TrainingSystem final {
    void update(Registry &, Spawner &);
};


}


#endif // FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
