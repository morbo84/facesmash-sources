#ifndef FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCORE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct GameRenderer;


class ScoreSystem final {
    static constexpr UInt8 interval = 50;

public:
    void update(Registry &, GameRenderer &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
