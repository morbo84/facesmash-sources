#ifndef FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCORE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct ScoreEvent;
struct GameRenderer;


struct ScoreSystem final {
    ScoreSystem();
    ~ScoreSystem();

    void receive(const ScoreEvent &) noexcept;

    void update(Registry &, GameRenderer &);

private:
    int score;
};


}


#endif // FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
