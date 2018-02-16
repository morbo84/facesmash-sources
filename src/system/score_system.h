#ifndef FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCORE_SYSTEM_H


#include "../event/event.hpp"
#include "../common/types.h"


namespace gamee {


struct ScoreSystem final {
    ScoreSystem() noexcept;
    ~ScoreSystem() noexcept;

    void receive(const SmashEvent &) noexcept;

    void update(Registry &);

private:
    int current;
    bool dirty;
    SmashEvent last;
};


}


#endif // FACE_SMASH_SYSTEM_SCORE_SYSTEM_H
