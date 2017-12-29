#ifndef FACE_SMASH_RESOURCE_FONT_RESOURCE_H
#define FACE_SMASH_RESOURCE_FONT_RESOURCE_H


#include <memory>
#include <SDL_ttf.h>
#include <SDL_pixels.h>
#include <entt/resource/handle.hpp>
#include <entt/resource/loader.hpp>
#include "texture_resource.h"


struct SDL_Renderer;
struct SDL_Texture;


namespace gamee {


struct TTFFontResource final {
    explicit TTFFontResource(TTF_Font *) noexcept;

    TTFFontResource(const TTFFontResource &) = delete;
    TTFFontResource(TTFFontResource &&) = delete;

    ~TTFFontResource() noexcept;

    TTFFontResource& operator=(const TTFFontResource &) = delete;
    TTFFontResource& operator=(TTFFontResource &&) = delete;

    operator TTF_Font *() const noexcept;

private:
    TTF_Font *font;
};


struct TTFFontLoader final: public entt::ResourceLoader<TTFFontLoader, TTFFontResource> {
    std::shared_ptr<TTFFontResource> load(const char *, int) const;
};


struct TTFFontTextureLoader final: public entt::ResourceLoader<TTFFontTextureLoader, SDLTextureResource> {
    std::shared_ptr<SDLTextureResource> load(const char *, SDL_Renderer *, TTF_Font *, SDL_Color) const;
};


using TTFFontHandle = entt::ResourceHandle<TTFFontResource>;


}


#endif // FACE_SMASH_RESOURCE_FONT_RESOURCE_H
