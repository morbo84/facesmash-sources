#include <cstddef>
#include <string>
#include <type_traits>
#include <SDL_surface.h>
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../game/game_renderer.h"
#include "wallpaper_system.h"


namespace gamee {


#ifdef __ANDROID__
void bindingWallpaperShare();
const std::string & bindingWallpaperOutputPath();
#else
void bindingWallpaperShare() {}
std::string bindingWallpaperOutputPath() { return {}; }
#endif


WallpaperSystem::WallpaperSystem()
    : probs{},
      copy{SDL_CreateRGBSurfaceWithFormat(0, detectorWidth, detectorHeight, SDL_BITSPERPIXEL(detectorFormat), detectorFormat)},
      type{FaceType::HAPPY},
      dirty{false},
      clear{false},
      share{false},
      acquire{false}
{
    Locator::Dispatcher::ref().sink<FaceEvent>().connect(this);
    Locator::Dispatcher::ref().sink<SceneChangeEvent>().connect(this);
    Locator::Dispatcher::ref().sink<WallpaperEvent>().connect(this);
}


WallpaperSystem::~WallpaperSystem() {
    SDL_FreeSurface(copy);
    Locator::Dispatcher::ref().sink<WallpaperEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<SceneChangeEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<FaceEvent>().disconnect(this);
}


void WallpaperSystem::receive(const FaceEvent &event) noexcept {
    auto pos = static_cast<std::underlying_type_t<FaceType>>(event.type);

    if(acquire && event.frame && probs[pos] < event.probability) {
        std::copy_n(event.frame.get(), copy->pitch * copy->h, static_cast<unsigned char *>(copy->pixels));
        probs[pos] = event.probability;
        type = event.type;
        dirty = true;
    }
}


void WallpaperSystem::receive(const SceneChangeEvent &event) noexcept {
    clear = (event.scene == SceneType::GAME_TUTORIAL);
    acquire = (event.scene == SceneType::THE_GAME);
}


void WallpaperSystem::receive(const WallpaperEvent &event) noexcept {
    share = (event.type == WallpaperEvent::Type::EXPORT);
}


void WallpaperSystem::update(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();
    auto wallpaperHandle = textureCache.handle("target/wallpaper");
    auto *prev = SDL_GetRenderTarget(renderer);

    renderer.target(*wallpaperHandle);

    auto printOverlays = [&]() {
        auto faceSmashHandle = textureCache.handle("str/share/facesmash");
        SDL_Rect faceSmashLabelRect{
                (wallpaperHandle->width() - faceSmashHandle->width()) / 2,
                detectorHeight - 5 * faceSmashHandle->height() / 4,
                faceSmashHandle->width(),
                faceSmashHandle->height()
        };

        auto hallOfFacesHandle = textureCache.handle("str/share/halloffaces");
        SDL_Rect hallOfFacesLabelRect{
                (wallpaperHandle->width() - hallOfFacesHandle->width()) / 2,
                detectorHeight + hallOfFacesHandle->height() / 4,
                hallOfFacesHandle->width(),
                hallOfFacesHandle->height()
        };

        SDL_RenderCopy(renderer, *faceSmashHandle, nullptr, &faceSmashLabelRect);
        SDL_RenderCopy(renderer, *hallOfFacesHandle, nullptr, &hallOfFacesLabelRect);
    };

    if(clear) {
        probs = {};
        renderer.clear(127_ui8, 127_ui8, 127_ui8);
        printOverlays();
    } else if(dirty) {
        auto update = [&, this](auto handle, auto row, auto col) {
            void *texture;
            int pitch;

            SDL_LockTexture(*handle, nullptr, &texture, &pitch);
            std::copy_n(static_cast<const unsigned char *>(copy->pixels), copy->h * copy->pitch, static_cast<unsigned char *>(texture));
            SDL_UnlockTexture(*handle);

            SDL_Rect dst { col*detectorWidth, row*detectorHeight, detectorWidth, detectorHeight };
            SDL_RenderCopy(renderer, *handle, nullptr, &dst);
        };

        switch(type) {
        case FaceType::ANGRY:
            update(textureCache.handle("wallpaper/angry"), 0, 0);
            break;
        case FaceType::DISGUSTED:
            update(textureCache.handle("wallpaper/disgusted"), 0, 1);
            break;
        case FaceType::FEARFUL:
            update(textureCache.handle("wallpaper/fearful"), 0, 2);
            break;
        case FaceType::HAPPY:
            update(textureCache.handle("wallpaper/happy"), 1, 0);
            break;
        case FaceType::SAD:
            update(textureCache.handle("wallpaper/sad"), 1, 1);
            break;
        case FaceType::SURPRISED:
            update(textureCache.handle("wallpaper/surprised"),1, 2);
            break;
        }

        printOverlays();
    } else if(share) {
        auto handle = textureCache.handle("target/wallpaper");
        auto *surface = SDL_CreateRGBSurfaceWithFormat(0, handle->width(), handle->height(), SDL_BITSPERPIXEL(internalFormat), internalFormat);

        SDL_RenderReadPixels(renderer, nullptr, internalFormat, surface->pixels, surface->pitch);
        SDL_SaveBMP(surface, bindingWallpaperOutputPath().c_str());
        bindingWallpaperShare();
        SDL_FreeSurface(surface);
    }

    renderer.target(prev);

    clear = false;
    dirty = false;
    share = false;
}


}
