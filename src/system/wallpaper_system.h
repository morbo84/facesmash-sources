#ifndef FACE_SMASH_SYSTEM_WALLPAPER_SYSTEM_H
#define FACE_SMASH_SYSTEM_WALLPAPER_SYSTEM_H


#include <array>
#include <cstddef>
#include <SDL_surface.h>
#include "../common/constants.h"
#include "../common/types.h"


namespace gamee {


struct GameRenderer;
struct FaceEvent;
struct SceneChangeEvent;
struct WallpaperEvent;


struct WallpaperSystem final {
    WallpaperSystem();
    ~WallpaperSystem();

    void onFace(const FaceEvent &) noexcept;
    void onSceneChanged(const SceneChangeEvent &) noexcept;
    void onWallpaper(const WallpaperEvent &) noexcept;

    void update(GameRenderer &);

private:
    std::array<float, numberOfFaces> probs;
    SDL_Surface *copy;
    FaceType type;
    bool dirty;
    bool clear;
    bool share;
    bool acquire;
};


}


#endif // FACE_SMASH_SYSTEM_WALLPAPER_SYSTEM_H
