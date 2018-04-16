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
static inline int appEventFilter(void *, SDL_Event *event) noexcept {
    // consume the event and force it to be dropped from the internal queue
    int queue = 0;

    switch(event->type) {
    case SDL_APP_TERMINATING:
        Locator::Dispatcher::ref().trigger<EnvEvent>(EnvEvent::Type::TERMINATING);
        break;
    case SDL_APP_LOWMEMORY:
        Locator::Dispatcher::ref().trigger<EnvEvent>(EnvEvent::Type::LOW_MEMORY);
        break;
    case SDL_APP_WILLENTERBACKGROUND:
        Locator::Dispatcher::ref().trigger<EnvEvent>(EnvEvent::Type::ENTERING_BACKGROUND);
        break;
    case SDL_APP_DIDENTERBACKGROUND:
        Locator::Dispatcher::ref().trigger<EnvEvent>(EnvEvent::Type::ENTERED_BACKGROUND);
        break;
    case SDL_APP_WILLENTERFOREGROUND:
        Locator::Dispatcher::ref().trigger<EnvEvent>(EnvEvent::Type::ENTERING_FOREGROUND);
        break;
    case SDL_APP_DIDENTERFOREGROUND:
        Locator::Dispatcher::ref().trigger<EnvEvent>(EnvEvent::Type::ENTERED_FOREGROUND);
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


void GameEnv::suspend() {
    Locator::Haptic::ref().pause();
    clock.pause();
}


void GameEnv::resume() {
    Locator::Haptic::ref().unpause();
    clock.unpause();
}


GameEnv::GameEnv() noexcept
    : errcode{ErrorCode::NONE}, renderer{nullptr}, loop{true}
{
    const Uint32 sdlFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_HAPTIC;
    const int sdlImageFlags = IMG_INIT_PNG;
    const int sdlMixerFlags = MIX_INIT_OGG;

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
        SDL_SetEventFilter(&appEventFilter, nullptr);
        // bind services to the surrounding environment
        Locator::Dispatcher::ref().connect<KeyboardEvent>(this);
        Locator::Dispatcher::ref().connect<EnvEvent>(this);
    }
}


GameEnv::~GameEnv() noexcept {
    Locator::Dispatcher::ref().disconnect<EnvEvent>(this);
    Locator::Dispatcher::ref().disconnect<KeyboardEvent>(this);
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


void GameEnv::receive(const KeyboardEvent &event) noexcept {
    switch(event.type) {
    case KeyboardEvent::Type::ESCAPE:
        Locator::Dispatcher::ref().enqueue<EnvEvent>(EnvEvent::Type::TERMINATING);
        break;
    default:
        // does nothing (suppress warnings)
        break;
    }
}


void GameEnv::receive(const EnvEvent &event) noexcept {
    switch(event.type) {
    case EnvEvent::Type::TERMINATING:
        // the user is exiting the game... :-(
        loop = false;
        break;
    case EnvEvent::Type::LOW_MEMORY:
        // well, I really don't know what to do here in such a small game...
        break;
    case EnvEvent::Type::ENTERING_BACKGROUND:
        suspend();
        break;
    case EnvEvent::Type::ENTERED_BACKGROUND:
        // nothing to do here, thanks anyway
        break;
    case EnvEvent::Type::ENTERING_FOREGROUND:
        resume();
        break;
    case EnvEvent::Type::ENTERED_FOREGROUND:
        // nothing to do here, thanks anyway
        break;
    }
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

            // poll events for the next loop
            Locator::InputHandler::ref().poll();

            // dispatch the events collected so far (if any)
            Locator::FaceBus::ref().dequeue();
            Locator::Dispatcher::ref().update();
            Locator::Permissions::ref().dequeue();

            // render the scene
            update(*renderer, elapsed);

            // try to keep the game at 30 FPS at least
            current = clock.ticks();
            if(previous + msPerFrame > current) {
                clock.delay(previous + msPerFrame - current);
            }
        }

        close();
    }

    return static_cast<std::underlying_type_t<ErrorCode>>(errcode);
}


}
