#ifndef FACE_SMASH_RESOURCE_TEXTURE_RESOURCE_H
#define FACE_SMASH_RESOURCE_TEXTURE_RESOURCE_H


#include <memory>
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

    SDLTextureResource& operator=(const SDLTextureResource &) = delete;
    SDLTextureResource& operator=(SDLTextureResource &&) = delete;

    operator SDL_Texture *() const noexcept;

private:
    SDL_Texture *texture;
};


struct SDLTextureLoader final: public entt::ResourceLoader<SDLTextureLoader, SDLTextureResource> {
    std::shared_ptr<SDLTextureResource> load(const char *, SDL_Renderer *) const;
};


using SDLTextureHandle = entt::ResourceHandle<SDLTextureResource>;


}


#endif // FACE_SMASH_RESOURCE_TEXTURE_RESOURCE_H
