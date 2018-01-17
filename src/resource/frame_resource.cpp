#include <SDL_render.h>
#include "frame_resource.h"


namespace gamee {


FrameTexture::FrameTexture(SDL_Renderer *renderer, int w, int h) noexcept {
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
}


FrameTexture::~FrameTexture() noexcept {
    SDL_DestroyTexture(texture);
}


FrameTexture::operator SDL_Texture *() const noexcept {
    return texture;
}


int FrameTexture::width() const noexcept {
    int width;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
    return width;
}


int FrameTexture::height() const noexcept {
    int height;
    SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &height);
    return height;
}


}
