#ifndef FACE_SMASH_SYSTEM_FACE_BUTTON_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_BUTTON_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct TouchEvent;


struct FaceButtonSystem final {
    FaceButtonSystem();
    ~FaceButtonSystem();

    void receive(const TouchEvent &) noexcept;

    void update(Registry &);

private:
    SDL_Point coord;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_UFACE_BUTTON_SYSTEM_H
