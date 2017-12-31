#ifndef FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCORE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct ScoreEvent;
struct GameRenderer;


class ScoreSystem final {
    static constexpr UInt8 interval = 50;

public:
    ScoreSystem();
    ~ScoreSystem();

    void receive(const ScoreEvent &) noexcept;

    void update(Registry &, GameRenderer &, delta_type);

private:
    UInt8 elapsed;
    UInt16 current;
    UInt16 score;
};


}


#endif // FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
