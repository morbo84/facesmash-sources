#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "recording_system.h"


namespace gamee {


void RecordingSystem::init(GameRenderer &renderer) {
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


void RecordingSystem::update(GameRenderer &renderer, delta_type delta, std::function<void(bool)> next) {
    auto &avMuxer = Locator::AvMuxer::ref();
    bool isRecording = false;

    if(avMuxer.recording()) {
        auto &textureCache = Locator::TextureCache::ref();
        auto logical = textureCache.handle("target/logical");
        auto recording = textureCache.handle("target/recording");

        accumulator += firstFrame ? 0_ui32 : delta;

        renderer.target(*recording);

        if(ready) {
            if(avMuxer.ready()) {
                SDL_RenderReadPixels(renderer, nullptr, 0, pixels.get(), pitch);
                avMuxer.frame(pixels.get(), accumulator);
                isRecording = true;
                firstFrame = false;
                ready = false;
            }
        } else {
            SDL_RenderCopy(renderer, *logical, nullptr, nullptr);
            ready = true;
        }

        renderer.target(*logical);
        renderer.clear();
        next(isRecording);

        renderer.target();
        SDL_RenderCopy(renderer, *logical, nullptr, nullptr);
        renderer.present();
    } else {
        accumulator = 0_ui32;
        firstFrame = true;
        ready = false;

        renderer.clear();
        next(isRecording);
        renderer.present();
    }
}


}
