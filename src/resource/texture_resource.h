#ifndef FACE_SMASH_RESOURCE_TEXTURE_RESOURCE_H
#define FACE_SMASH_RESOURCE_TEXTURE_RESOURCE_H


#include <memory>
#include <SDL.h>
#include <entt/resource/handle.hpp>
#include <entt/resource/loader.hpp>


struct SDL_Renderer;
struct SDL_Texture;


namespace gamee {


struct SDLTextureResource final {
    explicit SDLTextureResource(SDL_Texture *) noexcept;

    SDLTextureResource(const SDLTextureResource &) = delete;
    SDLTextureResource(SDLTextureResource &&) = delete;

    ~SDLTextureResource() noexcept;

    SDLTextureResource & operator=(const SDLTextureResource &) = delete;
    SDLTextureResource & operator=(SDLTextureResource &&) = delete;

    operator SDL_Texture *() const noexcept;

    int width() const noexcept;
    int height() const noexcept;

private:
    SDL_Texture *texture;
};


struct AssetTextureLoader final: public entt::ResourceLoader<AssetTextureLoader, SDLTextureResource> {
    std::shared_ptr<SDLTextureResource> load(const char *, SDL_Renderer *) const;
};


struct SDLTextureLoader final: public entt::ResourceLoader<SDLTextureLoader, SDLTextureResource> {
    std::shared_ptr<SDLTextureResource> load(SDL_Renderer *, Uint32, int, int, int) const;
};


using SDLTextureHandle = entt::ResourceHandle<SDLTextureResource>;


}


#endif // FACE_SMASH_RESOURCE_TEXTURE_RESOURCE_H
