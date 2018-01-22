#ifndef FACE_SMASH_GAME_GAME_RENDERER_H
#define FACE_SMASH_GAME_GAME_RENDERER_H


#include "../common/types.h"


struct SDL_Window;
struct SDL_Renderer;


namespace gamee {


struct GameRenderer final {
    GameRenderer();
    ~GameRenderer();

    void present() noexcept;
    void clear() noexcept;

    void target(SDL_Texture * = nullptr) noexcept;

    explicit operator bool() const noexcept;
    operator SDL_Renderer * () const noexcept;
    operator SDL_Window * () const noexcept;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};


}


#endif // FACE_SMASH_GAME_GAME_RENDERER_H
