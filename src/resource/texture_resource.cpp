#include <cassert>
#include <SDL_image.h>
#include <SDL_render.h>
#include "texture_resource.h"


namespace gamee {


SDLTextureResource::SDLTextureResource(SDL_Texture *texture) noexcept
    : texture{texture}
{
    assert(texture);
}


SDLTextureResource::~SDLTextureResource() noexcept {
    SDL_DestroyTexture(texture);
}


SDLTextureResource::operator SDL_Texture *() const noexcept {
    return texture;
}


int SDLTextureResource::width() const noexcept {
    int width;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
    return width;
}


int SDLTextureResource::height() const noexcept {
    int height;
    SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &height);
    return height;
}


std::shared_ptr<SDLTextureResource> AssetTextureLoader::load(const char *res, SDL_Renderer *renderer) const {
    std::shared_ptr<SDLTextureResource> ret;
    SDL_Surface *surface = IMG_Load(res);

    if(surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if(texture) {
            ret = std::make_shared<SDLTextureResource>(texture);
        }
    }

    return ret;
}


std::shared_ptr<SDLTextureResource> SDLTextureLoader::load(SDL_Renderer *renderer, Uint32 format, int access, int w, int h) const {
    std::shared_ptr<SDLTextureResource> ret;
    SDL_Texture *texture = SDL_CreateTexture(renderer, format, access, w, h);

    if(texture) {
        ret = std::make_shared<SDLTextureResource>(texture);
    }

    return ret;
}


}
