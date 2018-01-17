#ifndef FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
#define FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class PlayFactory;


struct TrainingSystem final {
    void update(Registry &, PlayFactory &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
