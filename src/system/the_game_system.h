#ifndef FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H


#include "../common/bag.h"
#include "../common/types.h"


namespace gamee {


struct TimeIsOverEvent;
class Spawner;


struct TheGameSystem final {
    TheGameSystem();
    ~TheGameSystem();

    void receive(const TimeIsOverEvent &) noexcept;

    void update(Registry &, Spawner &);

private:
    bool timeIsOver;
    FaceBag faceBag;
    ItemBag itemBag;
};


}


#endif // FACE_SMASH_SYSTEM_THE_GAME_SYSTEM_H
