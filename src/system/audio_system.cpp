#include <algorithm>
#include <SDL_mixer.h>
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "audio_system.h"


namespace gamee {


AudioSystem::AudioSystem()
    : remaining{0_ui32},
      explosion{0u},
      curr{SceneType::UNKNOWN},
      next{SceneType::UNKNOWN}
{
    Locator::Dispatcher::ref().connect<SceneChangeEvent>(this);
    Locator::Dispatcher::ref().connect<SmashEvent>(this);
    Locator::Dispatcher::ref().connect<EnvEvent>(this);
}


AudioSystem::~AudioSystem() {
    Locator::Dispatcher::ref().disconnect<SceneChangeEvent>(this);
    Locator::Dispatcher::ref().disconnect<SmashEvent>(this);
    Locator::Dispatcher::ref().disconnect<EnvEvent>(this);
}


void AudioSystem::receive(const SceneChangeEvent &event) noexcept {
    next = event.scene;
}


void AudioSystem::receive(const SmashEvent &event) noexcept {
    // combo is greater than 0 event in case of single smash, so why not? :-)
    explosion += (event.combo ? 1u : 0u);
}


void AudioSystem::receive(const EnvEvent &event) noexcept {
    switch(event.type) {
    case EnvEvent::Type::TERMINATING:
        Locator::Audio::ref().halt();
        break;
    case EnvEvent::Type::ENTERING_BACKGROUND:
        Locator::Audio::ref().pause();
        break;
    case EnvEvent::Type::ENTERING_FOREGROUND:
        Locator::Audio::ref().resume();
        break;
    case EnvEvent::Type::ENTERED_BACKGROUND:
    case EnvEvent::Type::ENTERED_FOREGROUND:
        // nothing to do here
        break;
    case EnvEvent::Type::LOW_MEMORY:
        // enjoy the last chunks of battery, my friend!!
        break;
    }
}


void AudioSystem::update(Registry &registry, delta_type delta) {
    if(explosion) {
        remaining -= std::min(remaining, delta);

        if(!remaining) {
            // TODO play explosion
            --explosion;
        }
   }

    if(curr != next) {
        // TODO

        switch(next) {
        case SceneType::UNKNOWN:
            // TODO
            break;
        case SceneType::EXIT:
            // TODO
            break;
        case SceneType::SPLASH_SCREEN:
            // TODO
            break;
        case SceneType::MENU_PAGE:
            // TODO
            break;
        case SceneType::CREDITS_PAGE:
            // TODO
            break;
        case SceneType::SUPPORT_PAGE:
            // TODO
            break;
        case SceneType::SETTINGS_PAGE:
            // TODO
            break;
        case SceneType::GAME_TUTORIAL:
            // TODO
            break;
        case SceneType::THE_GAME:
            // TODO
            break;
        case SceneType::GAME_OVER:
            // TODO
            break;
        case SceneType::TRAINING_TUTORIAL:
            // TODO
            break;
        case SceneType::TRAINING:
            // TODO
            break;
        case SceneType::LOGIN_PLEASE:
            // TODO
            break;
        }

        curr = next;
    }
}


}