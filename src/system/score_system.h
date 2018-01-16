#ifndef FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCORE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


struct ScoreSystem final {
    void update(Registry &);
};


}


#endif // FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
