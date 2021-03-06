#include <SDL_timer.h>
#include "clock.h"


namespace gamee {


Clock::Clock()
    : previous{SDL_GetTicks()},
      gap{0},
      paused{false}
{}


void Clock::pause() noexcept {
    if(!paused) {
        previous = SDL_GetTicks();
        paused = true;
    }
}


void Clock::unpause() noexcept {
    if(paused) {
        gap += (SDL_GetTicks() - previous);
        paused = false;
    }
}


delta_type Clock::ticks() noexcept {
    auto current = SDL_GetTicks();

    if(paused) {
        gap += (current - previous);
        previous = current;
    }

    return (current - gap);
}


delta_type Clock::time() const noexcept {
    return SDL_GetTicks();
}


void Clock::delay(delta_type delta) const noexcept {
    SDL_Delay(delta);
}


}
