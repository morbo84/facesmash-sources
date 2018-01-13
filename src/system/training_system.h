#ifndef FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
#define FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class Factory;


struct TrainingSystem final {
    void update(Registry &, Factory &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
