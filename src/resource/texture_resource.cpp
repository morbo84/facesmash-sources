#include <SDL_image.h>
#include <SDL_render.h>
#include "texture_resource.h"


namespace gamee {


SDLTextureResource::SDLTextureResource(SDL_Texture *texture) noexcept
    : texture{texture}
{}


SDLTextureResource::~SDLTextureResource() noexcept {
    SDL_DestroyTexture(texture);
}


SDLTextureResource::operator SDL_Texture *() const noexcept {
    return texture;
}


std::shared_ptr<SDLTextureResource> SDLTextureLoader::load(const char *res, SDL_Renderer *renderer) const {
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


}
