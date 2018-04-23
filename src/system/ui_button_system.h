#ifndef FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
#define FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H


#include <optional>
#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct TouchEvent;
struct GameServicesEvent;


struct UIButtonSystem final {
    UIButtonSystem();
    ~UIButtonSystem();

    void receive(const TouchEvent &) noexcept;
    void receive(const GameServicesEvent &) noexcept;

    void showAchievements();
    void showLeaderboard();

    void update(Registry &);

private:
    SDL_Point coord;
    bool dirty;
    void (UIButtonSystem:: *pending)();
    std::optional<GameServicesEvent> gsEvent;
};


}


#endif // FACE_SMASH_SYSTEM_UI_BUTTON_SYSTEM_H
