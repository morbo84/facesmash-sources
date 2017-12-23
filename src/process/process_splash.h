#ifndef FACE_SMASH_PROCESS_PROCESS_SPLASH_H
#define FACE_SMASH_PROCESS_PROCESS_SPLASH_H


#include "../types/types.hpp"


struct SDL_Thread;


namespace gamee {


class GameRenderer;


class ProcessSplash final: public Process<ProcessSplash> {
    static constexpr float duration = 5000;
    static constexpr float logo = 3000;

    void updateRainbow(GameRenderer &);
    void updateGamee(GameRenderer &);
    void updateBanner(GameRenderer &);

    static int assets(void *);

public:
    void init(void *);
    void update(delta_type, void *);

private:
    delta_type elapsed{0};
    SDL_Thread *loader;
};


}


#endif // FACE_SMASH_PROCESS_PROCESS_SPLASH_H
