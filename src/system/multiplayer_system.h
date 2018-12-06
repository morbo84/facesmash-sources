#ifndef FACE_SMASH_SYSTEM_MULTIPLAYER_SYSTEM_H
#define FACE_SMASH_SYSTEM_MULTIPLAYER_SYSTEM_H


#include <memory>
#include <SDL_rect.h>
#include "../common/constants.h"
#include "../common/types.h"
#include "../event/event.hpp"


struct SDL_Surface;


namespace gamee {


class MultiplayerSystem final {
    static constexpr auto share_interval = 30_ui32;

    void acquire(Registry &);

public:
    MultiplayerSystem();
    ~MultiplayerSystem();

    void update(Registry &, delta_type);

    void onSceneChange(const SceneChangeEvent &) noexcept;
    void onCameraInit(const CameraInitEvent &) noexcept;

private:
    delta_type elapsed;
    bool shareStream;
    int current;
    SDL_Rect clip;
    SDL_Surface *cpy{nullptr};
    SDL_Surface *buf{nullptr};
};


}


#endif // FACE_SMASH_SYSTEM_MULTIPLAYER_SYSTEM_H
