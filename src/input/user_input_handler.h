#ifndef FACE_SMASH_INPUT_USER_INPUT_HANDLER_H
#define FACE_SMASH_INPUT_USER_INPUT_HANDLER_H


#include "input_handler.h"


union SDL_Event;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
struct SDL_TouchFingerEvent;
struct SDL_KeyboardEvent;


namespace gamee {


class UserInputHandler final: public InputHandler {
    void handle(const SDL_Event &) noexcept;
    void mousemotion(const SDL_MouseMotionEvent &) noexcept;
    void mousebutton(const SDL_MouseButtonEvent &) noexcept;
    void touchfinger(const SDL_TouchFingerEvent &) noexcept;
    void keyboard(const SDL_KeyboardEvent &) noexcept;
    void quit() noexcept;

public:
    void poll() noexcept override;
    void wait() noexcept override;
    void waitFor(int) noexcept override;
};


}


#endif // FACE_SMASH_INPUT_USER_INPUT_HANDLER_H
