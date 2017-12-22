#include <SDL_events.h>
#include "../event/event.hpp"
#include "../settings/settings.h"
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
    float x = event.x;
    float y = event.y;
    Locator::Dispatcher::ref().enqueue<TouchEvent>(x, y);
}


void UserInputHandler::touchfinger(const SDL_TouchFingerEvent &event) noexcept {
    Settings settings;
    auto x = event.x * settings.logicalWidth();
    auto y = event.y * settings.logicalHeight();
    Locator::Dispatcher::ref().enqueue<TouchEvent>(x, y);
}


void UserInputHandler::keyboard(const SDL_KeyboardEvent &event) noexcept {
    switch(event.keysym.sym) {
    case SDLK_ESCAPE:
        Locator::Dispatcher::ref().enqueue<EnvEvent>(EnvEvent::Type::TERMINATING);
        break;
    default:
        // silent warnings
        (void)event;
    }
}


void UserInputHandler::quit() noexcept {
    Locator::Dispatcher::ref().enqueue<EnvEvent>(EnvEvent::Type::TERMINATING);
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
