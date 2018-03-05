#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "../event/event.hpp"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "../service/av_recorder_android.h"
#include "../service/av_recorder_null.h"
#include "recording_system.h"


namespace gamee {


RecordingSystem::RecordingSystem()
    : pixels{nullptr},
      accumulator{0_ui32},
      firstFrame{true},
      pitch{0}
{
    Locator::Dispatcher::ref().connect<RecorderEvent>(this);
}


RecordingSystem::~RecordingSystem() {
    Locator::Dispatcher::ref().disconnect<RecorderEvent>(this);
}


void RecordingSystem::init() {
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


void RecordingSystem::receive(const RecorderEvent &event) noexcept {
    switch(event.type) {
#ifdef __ANDROID__
    case RecorderEvent::Type::ENABLE:
        Locator::AvRecorder::set<AvRecorderAndroid>();
        break;
#else
    case RecorderEvent::Type::ENABLE:
#endif
    case RecorderEvent::Type::DISABLE:
        Locator::AvRecorder::set<AvRecorderNull>();
        break;
    case RecorderEvent::Type::EXPORT:
        Locator::AvRecorder::ref().exportMedia();
        break;
    }
}


void RecordingSystem::update(GameRenderer &renderer, delta_type delta, std::function<void(void)> next) {
    auto &avRecorder = Locator::AvRecorder::ref();

    if(avRecorder.recording()) {
        auto &textureCache = Locator::TextureCache::ref();
        auto logical = textureCache.handle("target/logical");
        auto recording = textureCache.handle("target/recording");

        accumulator += firstFrame ? 0_ui32 : delta;

        renderer.target(*logical);
        renderer.clear();
        next();

        renderer.target();
        SDL_RenderCopy(renderer, *logical, nullptr, nullptr);
        renderer.present();

        if(avRecorder.ready()) {
            renderer.target(*recording);
            SDL_RenderCopy(renderer, *logical, nullptr, nullptr);
            SDL_RenderReadPixels(renderer, nullptr, internalFormat, pixels.get(), pitch);
            renderer.target();

            avRecorder.frame(pixels.get(), accumulator);
            firstFrame = false;
        }
    } else {
        accumulator = 0_ui32;
        firstFrame = true;

        renderer.clear();
        next();
        renderer.present();
    }
}


}
