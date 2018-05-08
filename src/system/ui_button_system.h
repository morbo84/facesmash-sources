#ifndef FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
#define FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct TouchEvent;


struct UIButtonSystem final {
    UIButtonSystem();
    ~UIButtonSystem();

    void receive(const TouchEvent &) noexcept;

    void showAchievements();
    void showLeaderboard();

    void updateLoginButton(Registry &);
    void update(Registry &);

private:
    SDL_Point coord;
    bool dirty;
    void (UIButtonSystem:: *pending)();
};


}


#endif // FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
