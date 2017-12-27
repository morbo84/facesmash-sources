#include <utility>
#include <SDL_render.h>
#include "font_resource.h"


namespace gamee {


TTFFontResource::TTFFontResource(TTF_Font *font) noexcept
    : font{font}
{}


TTFFontResource::~TTFFontResource() noexcept {
    TTF_CloseFont(font);
}


TTFFontResource::operator TTF_Font *() const noexcept {
    return font;
}


TTFFontTextureResource::TTFFontTextureResource(SDL_Texture *texture, int w, int h) noexcept
    : texture{texture}, w{w}, h{h}
{}


TTFFontTextureResource::~TTFFontTextureResource() noexcept {
    SDL_DestroyTexture(texture);
}


TTFFontTextureResource::operator SDL_Texture *() const noexcept {
    return texture;
}


int TTFFontTextureResource::width() const noexcept {
    return w;
}


int TTFFontTextureResource::height() const noexcept {
    return h;
}


std::shared_ptr<TTFFontResource> TTFFontLoader::load(const char *res, int pt) const {
    std::shared_ptr<TTFFontResource> ret;
    TTF_Font *font = TTF_OpenFont(res, pt);

    if(font) {
        ret = std::make_shared<TTFFontResource>(font);
    }

    return ret;
}


std::shared_ptr<TTFFontTextureResource> TTFFontTextureLoader::load(const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color fg) const {
    std::shared_ptr<TTFFontTextureResource> ret;
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, fg);

    if(surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if(texture) {
            int width, height;
            TTF_SizeText(font, text, &width, &height);
            ret = std::make_shared<TTFFontTextureResource>(texture, width, height);
        }
    }

    return ret;
}


}
