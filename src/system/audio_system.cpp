#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../service/audio_null.h"
#include "../service/audio_sdl.h"
#include "audio_system.h"


namespace gamee {


void AudioSystem::start() noexcept {
    Locator::Audio::set<AudioSDL>();

    auto &audio = Locator::Audio::ref();

    if(audio.paused()) {
        audio.resume();
    } else {
        curr = AudioMusicType::AUDIO_MUSIC_UNKNOWN;
        next = AudioMusicType::AUDIO_MUSIC_RELAX;
    }
}


void AudioSystem::stop() noexcept {
    auto &audio = Locator::Audio::ref();

    if(audio.playing()) {
        audio.pause();
    }

    Locator::Audio::set<AudioNull>();
}


AudioSystem::AudioSystem()
    : curr{AudioMusicType::AUDIO_MUSIC_UNKNOWN},
      next{AudioMusicType::AUDIO_MUSIC_UNKNOWN}
{
    Locator::Dispatcher::ref().connect<AudioEvent>(this);
    Locator::Dispatcher::ref().connect<AudioMusicEvent>(this);
}


AudioSystem::~AudioSystem() {
    Locator::Dispatcher::ref().disconnect<AudioMusicEvent>(this);
    Locator::Dispatcher::ref().disconnect<AudioEvent>(this);
}


void AudioSystem::receive(const AudioEvent &event) noexcept {
    switch (event.type) {
    case AudioEvent::Type::START:
        start();
        break;
    case AudioEvent::Type::STOP:
        stop();
        break;
    }
}


void AudioSystem::receive(const AudioMusicEvent &event) noexcept {
    if(event.playOrEntering) {
        next = event.type;
    } else if(event.type != curr) {
        Locator::Audio::ref().fadeOut(fadeOut);
    }
}


void AudioSystem::update() {
    if(curr != next) {
        auto &musicCache = Locator::AudioMusicCache::ref();
        auto &audio = Locator::Audio::ref();

        if(curr == AudioMusicType::AUDIO_MUSIC_UNKNOWN) {
            switch(next) {
            case AudioMusicType::AUDIO_MUSIC_PLAY:
                audio.play(musicCache.handle("music/play"));
                break;
            case AudioMusicType::AUDIO_MUSIC_RELAX:
                audio.play(musicCache.handle("music/relax"));
                break;
            default:
                // do not set audio in this case
                break;
            }
        } else {
            switch(next) {
            case AudioMusicType::AUDIO_MUSIC_PLAY:
                audio.fadeIn(musicCache.handle("music/play"), fadeIn);
                break;
            case AudioMusicType::AUDIO_MUSIC_RELAX:
                audio.fadeIn(musicCache.handle("music/relax"), fadeIn);
                break;
            default:
                // do not set audio in this case
                break;
            }
        }
    }

    curr = next;
}


}
