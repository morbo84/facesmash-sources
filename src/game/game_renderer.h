#ifndef FACE_SMASH_GAME_GAME_RENDERER_H
#define FACE_SMASH_GAME_GAME_RENDERER_H


#include "../types/types.hpp"


struct SDL_Window;
struct SDL_Renderer;


namespace gamee {


struct GameRenderer final {
    GameRenderer();
    ~GameRenderer();

    void present() noexcept;
    void clear() noexcept;

    explicit operator bool() const noexcept;
    operator SDL_Renderer * () const noexcept;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
};


}


#endif // FACE_SMASH_GAME_GAME_RENDERER_H
