#ifndef FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCORE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct ScoreEvent;


struct ScoreSystem final {
    ScoreSystem();
    ~ScoreSystem();

    void receive(const ScoreEvent &) noexcept;

    void update(Registry &);
};


}


#endif // FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
