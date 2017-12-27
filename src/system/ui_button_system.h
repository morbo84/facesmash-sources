#ifndef FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
#define FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H


#include <SDL_rect.h>
#include "../event/event.hpp"
#include "../types/types.hpp"


namespace gamee {


struct TouchEvent;


struct UIButtonSystem final {
    UIButtonSystem();
    ~UIButtonSystem();

    void receive(const TouchEvent &) noexcept;

    void update(Registry &);

private:
    SDL_Point coord;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
