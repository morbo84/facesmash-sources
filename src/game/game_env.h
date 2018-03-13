#ifndef FACE_SMASH_GAME_GAME_ENV_H
#define FACE_SMASH_GAME_GAME_ENV_H


#include <memory>
#include "../common/types.h"
#include "../time/clock.h"
#include "game_renderer.h"


namespace gamee {


struct KeyboardEvent;
struct EnvEvent;


class GameEnv {
    static constexpr delta_type clampOverElapsed = 25;
    static constexpr delta_type msPerFrame = 16;

    virtual void suspend();
    virtual void resume();

    virtual void init(GameRenderer &) = 0;
    virtual void close() = 0;

    virtual void update(GameRenderer &, delta_type) = 0;

public:
    enum class ErrorCode: int {
        NONE = 0,
        UNDERLYING_LIBRARY_INITIALIZATION,
        RENDERER_INITIALIZATION
    };

    GameEnv() noexcept;
    virtual ~GameEnv() noexcept;

    GameEnv(const GameEnv &) = delete;
    GameEnv(GameEnv &&) = delete;

    GameEnv & operator=(const GameEnv &) = delete;
    GameEnv & operator=(GameEnv &&) = delete;

    bool valid() const noexcept;
    ErrorCode error() const noexcept;

    void receive(const KeyboardEvent &) noexcept;
    void receive(const EnvEvent &) noexcept;

    int exec() noexcept;

private:
    // internal TTF like WasInit variables for SDL2, Image and Mixer
    bool SDL_WasInit;
    bool IMG_WasInit;
    bool Mix_WasInit;
    bool Audio_WasInit;
    // actual game data members
    ErrorCode errcode;
    std::unique_ptr<GameRenderer> renderer;
    Clock clock;
    bool loop;
};


}


#endif // FACE_SMASH_GAME_GAME_ENV_H
