#include <memory>
#include <SDL_render.h>
#include <SDL_pixels.h>
#include "../common/constants.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "recording_system.h"


namespace gamee {


void RecordingSystem::update(GameRenderer &renderer, delta_type delta) {
    auto &avMuxer = Locator::AvMuxer::ref();

    if(avMuxer.recording()) {
        constexpr auto format = SDL_PIXELFORMAT_RGBA32;
        constexpr auto size = logicalWidth * logicalHeight * SDL_BYTESPERPIXEL(format);
        constexpr auto pitch = logicalWidth * SDL_BYTESPERPIXEL(format);

        std::unique_ptr<unsigned char[]> pixels{new unsigned char[size]};
        SDL_RenderReadPixels(renderer, nullptr, 0, pixels.get(), pitch);
        avMuxer.frame(std::move(pixels), delta);
    }
}


}
