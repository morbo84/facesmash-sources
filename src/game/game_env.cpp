#include <type_traits>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_events.h>
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "game_env.h"


namespace gamee {


// we use this static function as event filter on mobile (see SDL docs)
int GameEnv::appEventFilter(void *ptr, SDL_Event *event) noexcept {
    auto &env = *static_cast<GameEnv *>(ptr);

    // consume the event and force it to be dropped from the internal queue
    int queue = 0;

    switch(event->type) {
    case SDL_APP_TERMINATING:
        // the user is exiting the game... :-(
        Locator::GameServices::ref().multiplayer().terminateMatch();
        env.loop = false;
        break;
    case SDL_APP_LOWMEMORY:
        // well, I really don't know what to do here in such a small game...
        break;
    case SDL_APP_WILLENTERBACKGROUND:
        env.clock.pause();
        break;
    case SDL_APP_DIDENTERBACKGROUND:
        env.clock.pause();
        break;
    case SDL_APP_WILLENTERFOREGROUND:
        env.clock.unpause();
        break;
    case SDL_APP_DIDENTERFOREGROUND:
        env.clock.unpause();
        break;
    case SDL_MOUSEMOTION:
        queue = event->motion.which == SDL_TOUCH_MOUSEID ? 0 : 1;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        queue = event->button.which == SDL_TOUCH_MOUSEID ? 0 : 1;
        break;
    default:
        queue = 1;
    }

    return queue;
}


GameEnv::GameEnv() noexcept
    : errcode{ErrorCode::NONE}, renderer{nullptr}, loop{true}
{
    const Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_HAPTIC;
    const int sdlImageFlags = IMG_INIT_PNG;
    // lovely bug - https://bugzilla.libsdl.org/show_bug.cgi?id=3929
    const int sdlMixerFlags =
            (Mix_Linked_Version()->major == 2 && Mix_Linked_Version()->minor == 0 && Mix_Linked_Version()->patch == 2)
            ? 0 // cross your finger, who knows what's supported
            : MIX_INIT_OGG;

    SDL_WasInit = (SDL_Init(sdlFlags) == 0);
    IMG_WasInit = ((IMG_Init(sdlImageFlags) & sdlImageFlags) == sdlImageFlags);
    Mix_WasInit = ((Mix_Init(sdlMixerFlags) & sdlMixerFlags) == sdlMixerFlags);
    Audio_WasInit = (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != -1);
    TTF_Init();

    renderer = std::make_unique<GameRenderer>();

    if(!SDL_WasInit || !IMG_WasInit || !Mix_WasInit || !Audio_WasInit || !TTF_WasInit()) {
        errcode = ErrorCode::UNDERLYING_LIBRARY_INITIALIZATION;
    } else if(!*renderer) {
        errcode = ErrorCode::RENDERER_INITIALIZATION;
    } else {
        // set event filter on SDL events
        SDL_SetEventFilter(&appEventFilter, this);
        // bind services to the surrounding environment
        Locator::Dispatcher::ref().sink<KeyboardEvent>().connect<&GameEnv::onKeyboardEvent>(this);
        Locator::Dispatcher::ref().sink<QuitEvent>().connect<&GameEnv::onQuitEvent>(this);
    }
}


GameEnv::~GameEnv() noexcept {
    Locator::Dispatcher::ref().sink<QuitEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<KeyboardEvent>().disconnect(this);
    SDL_SetEventFilter(nullptr, nullptr);
    if(TTF_WasInit()) { TTF_Quit(); }
    if(Audio_WasInit) { Mix_CloseAudio(); }
    if(Mix_WasInit) { Mix_Quit(); }
    if(IMG_WasInit) { IMG_Quit(); }
    if(SDL_WasInit) { SDL_Quit(); }
}


bool GameEnv::valid() const noexcept {
    return ErrorCode::NONE == errcode;
}


GameEnv::ErrorCode GameEnv::error() const noexcept {
    return errcode;
}


void GameEnv::onKeyboardEvent(const KeyboardEvent &event) noexcept {
    switch(event.type) {
    case KeyboardEvent::Type::ESCAPE:
        Locator::Dispatcher::ref().enqueue<QuitEvent>();
        break;
    default:
        // does nothing (suppress warnings)
        break;
    }
}


void GameEnv::onQuitEvent(const QuitEvent &event) noexcept {
    loop = false;
}


int GameEnv::exec() noexcept {
    /**
     * The following loop is aimed to save the users' batteries and it
     * could be quite uncommon for the readers that usually don't work on
     * mobile games. It should be a good compromise between speed and
     * battery consumption... or I'm wrong and that's the worst loop ever
     * seen before!! :-)
     */

    // in case of errors, the game won't start
    loop = loop && valid();

    if(loop) {
        // initialize the game
        init(*renderer);

        // initialize game times
        delta_type previous = clock.ticks();

        while(loop) {
            delta_type current = clock.ticks();
            delta_type elapsed = current - previous;

            previous = current;

            // dispatch the events collected so far (if any)
            Locator::FaceBus::ref().dequeue();
            Locator::Permissions::ref().dequeue();
            Locator::Billing::ref().dequeue();
            Locator::Dispatcher::ref().update();

            // render the scene
            update(*renderer, elapsed);

            // try to keep the game at 30 FPS at least
            current = clock.ticks();
            if(previous + msPerFrame > current) {
                clock.delay(previous + msPerFrame - current);
            }

            // poll events for the next loop
            Locator::InputHandler::ref().poll();
        }

        close();
    }

    return static_cast<std::underlying_type_t<ErrorCode>>(errcode);
}


}
