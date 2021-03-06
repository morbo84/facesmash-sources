#include <SDL_events.h>
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "user_input_handler.h"


namespace gamee {


void UserInputHandler::handle(const SDL_Event &event) noexcept {
    switch(event.type) {
    case SDL_FINGERDOWN:
        touchfinger(event.tfinger);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mousebutton(event.button);
        break;
    case SDL_KEYUP:
        keyboard(event.key);
        break;
    case SDL_QUIT:
        quit();
        break;
    }
}


void UserInputHandler::mousebutton(const SDL_MouseButtonEvent &event) noexcept {
    Locator::Dispatcher::ref().enqueue<TouchEvent>(event.x, event.y);
}


void UserInputHandler::touchfinger(const SDL_TouchFingerEvent &event) noexcept {
    const int x = event.x * logicalWidth;
    const int y = event.y * logicalHeight;
    Locator::Dispatcher::ref().enqueue<TouchEvent>(x, y);
}


void UserInputHandler::keyboard(const SDL_KeyboardEvent &event) noexcept {
    switch(event.keysym.sym) {
    case SDLK_ESCAPE:
        Locator::Dispatcher::ref().enqueue<KeyboardEvent>(KeyboardEvent::Type::ESCAPE);
        break;
    case SDLK_BACKSPACE:
    case SDLK_AC_BACK:
        Locator::Dispatcher::ref().enqueue<KeyboardEvent>(KeyboardEvent::Type::BACK);
        break;
    default:
        // silent warnings
        (void)event;
    }
}


void UserInputHandler::quit() noexcept {
    Locator::Dispatcher::ref().enqueue<QuitEvent>();
}


void UserInputHandler::poll() noexcept {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        handle(event);
    }
}


void UserInputHandler::wait() noexcept {
    SDL_Event event;
    if(SDL_WaitEvent(&event)) {
        handle(event);
    }
}


void UserInputHandler::waitFor(int ms) noexcept {
    SDL_Event event;
    if(SDL_WaitEventTimeout(&event, ms)) {
        handle(event);
    }
}


}
