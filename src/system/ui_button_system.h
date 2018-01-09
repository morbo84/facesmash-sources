#ifndef FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
#define FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"
#include "../event/event.hpp"


namespace gamee {


struct TouchEvent;


struct UIButtonSystem final {
    UIButtonSystem();
    ~UIButtonSystem();

    void switchAudio(Registry &, entity_type);

    void receive(const TouchEvent &) noexcept;

    void update(Registry &);

private:
    SDL_Point coord;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
