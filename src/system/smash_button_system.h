#ifndef FACE_SMASH_SYSTEM_SMASH_BUTTON_SYSTEM_H
#define FACE_SMASH_SYSTEM_SMASH_BUTTON_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct TouchEvent;


struct SmashButtonSystem final {
    SmashButtonSystem();
    ~SmashButtonSystem();

    void receive(const TouchEvent &) noexcept;

    void update(Registry &);

private:
    SDL_Point coord;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_SMASH_BUTTON_SYSTEM_H
