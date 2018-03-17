#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "../service/av_recorder_android.h"
#include "../service/av_recorder_null.h"
#include "av_recorder_system.h"


namespace gamee {


AvRecorderSystem::AvRecorderSystem()
    : pixels{nullptr},
      accumulator{0_ui32},
      frameTime{0_ui32},
      firstFrame{true},
      hasFrame{false},
      pitch{0}
{
    Locator::Dispatcher::ref().connect<AvRecorderEvent>(this);
}


AvRecorderSystem::~AvRecorderSystem() {
    Locator::Dispatcher::ref().disconnect<AvRecorderEvent>(this);
}


void AvRecorderSystem::init() {
    auto &textureCache = Locator::TextureCache::ref();
    auto recording = textureCache.handle("target/recording");

    Uint32 format;
    int access;
    int w;
    int h;

    SDL_QueryTexture(*recording, &format, &access, &w, &h);

    const auto size = w * h * SDL_BYTESPERPIXEL(format);
    pixels.reset(new unsigned char [size]);
    pitch = w * SDL_BYTESPERPIXEL(format);
}


void AvRecorderSystem::receive(const AvRecorderEvent &event) noexcept {
    switch(event.type) {
#ifdef __ANDROID__
    case AvRecorderEvent::Type::ENABLE:
        Locator::AvRecorder::set<AvRecorderAndroid>();
        break;
#else
    case AvRecorderEvent::Type::ENABLE:
#endif
    case AvRecorderEvent::Type::DISABLE:
        Locator::AvRecorder::set<AvRecorderNull>();
        break;
    case AvRecorderEvent::Type::EXPORT:
        Locator::AvRecorder::ref().exportMedia();
        break;
    }
}


void AvRecorderSystem::receive(const PermissionEvent &event) noexcept {
    if(event.permission == PermissionType::STORAGE && event.result == PermissionStatus::GRANTED) {
        Locator::Dispatcher::ref().enqueue<AvRecorderEvent>(AvRecorderEvent::Type::EXPORT);
    }
}


void AvRecorderSystem::update(GameRenderer &renderer, delta_type delta, std::function<void(void)> next) {
    auto &avRecorder = Locator::AvRecorder::ref();

    if(avRecorder.recording()) {
        auto &textureCache = Locator::TextureCache::ref();
        auto logical = textureCache.handle("target/logical");
        auto recording = textureCache.handle("target/recording");

        accumulator += firstFrame ? 0_ui32 : delta;

        if(hasFrame) {
            if(avRecorder.ready()) {
                renderer.target(*recording);
                SDL_RenderReadPixels(renderer, nullptr, internalFormat, pixels.get(), pitch);
                avRecorder.frame(pixels.get(), frameTime);
                firstFrame = false;
                hasFrame = false;
            }

            renderer.target();
            renderer.clear();
            next();
        } else {
            renderer.target(*logical);
            renderer.clear();
            next();

            renderer.target(*recording);
            SDL_RenderCopy(renderer, *logical, nullptr, nullptr);

            renderer.target();
            SDL_RenderCopy(renderer, *logical, nullptr, nullptr);

            frameTime += accumulator;
            accumulator = 0_ui32;
            hasFrame = true;
        }
    } else {
        accumulator = 0_ui32;
        frameTime = 0_ui32;
        firstFrame = true;
        hasFrame = false;

        renderer.target();
        renderer.clear();
        next();
    }

    renderer.present();
}


}
