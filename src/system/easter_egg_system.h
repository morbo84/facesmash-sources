#ifndef FACE_SMASH_SYSTEM_EASTER_EGG_SYSTEM_H
#define FACE_SMASH_SYSTEM_EASTER_EGG_SYSTEM_H


#include <SDL_rect.h>
#include "../common/bag.h"
#include "../common/types.h"
#include "../event/event.hpp"


namespace gamee {


struct PlayFactory;
struct TouchEvent;


struct EasterEggSystem final {
    EasterEggSystem();
    ~EasterEggSystem();

    void receive(const TouchEvent &) noexcept;

    void update(Registry &, PlayFactory &);

private:
    FaceBag bag;
    SDL_Point coord;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_EASTER_EGG_SYSTEM_H
