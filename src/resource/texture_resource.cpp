#include <SDL_image.h>
#include <SDL_render.h>
#include "texture_resource.h"


namespace gamee {


SDLTextureResource::SDLTextureResource(SDL_Texture *texture) noexcept
    : texture{texture}, w{0u}, h{0u}
{}


SDLTextureResource::~SDLTextureResource() noexcept {
    SDL_DestroyTexture(texture);
}


SDLTextureResource::operator SDL_Texture *() const noexcept {
    return texture;
}


int SDLTextureResource::width() const noexcept {
    return w;
}


int SDLTextureResource::height() const noexcept {
    return h;
}


void SDLTextureResource::width(int value) noexcept {
    w = value;
}


void SDLTextureResource::height(int value) noexcept {
    h = value;
}


std::shared_ptr<SDLTextureResource> SDLTextureLoader::load(const char *res, SDL_Renderer *renderer, int w, int h) const {
    std::shared_ptr<SDLTextureResource> ret;
    SDL_Surface *surface = IMG_Load(res);

    if(surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if(texture) {
            ret = std::make_shared<SDLTextureResource>(texture);
            ret->width(w);
            ret->height(h);
        }
    }

    return ret;
}


std::shared_ptr<SDLTextureResource> SDLStreamingTextureLoader::load(SDL_Renderer *renderer, int w, int h) const {
    std::shared_ptr<SDLTextureResource> ret;
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_NV21, SDL_TEXTUREACCESS_STREAMING, w, h);

    if(texture) {
        ret = std::make_shared<SDLTextureResource>(texture);
        ret->width(w);
        ret->height(h);
    }

    return ret;
}


}
