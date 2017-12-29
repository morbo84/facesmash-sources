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


std::shared_ptr<TTFFontResource> TTFFontLoader::load(const char *res, int pt) const {
    std::shared_ptr<TTFFontResource> ret;
    TTF_Font *font = TTF_OpenFont(res, pt);

    if(font) {
        ret = std::make_shared<TTFFontResource>(font);
    }

    return ret;
}


std::shared_ptr<SDLTextureResource> TTFFontTextureLoader::load(const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color fg) const {
    std::shared_ptr<SDLTextureResource> ret;
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, fg);

    if(surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if(texture) {
            ret = std::make_shared<SDLTextureResource>(texture);

            int w, h;
            TTF_SizeText(font, text, &w, &h);
            ret->width(w);
            ret->height(h);
        }
    }

    return ret;
}


}
