#ifndef FACE_SMASH_PROCESS_PROCESS_SPLASH_H
#define FACE_SMASH_PROCESS_PROCESS_SPLASH_H


#include "../common/types.h"


namespace gamee {


class GameRenderer;


class ProcessSplash final: public Process<ProcessSplash> {
    static constexpr float duration = 3000;
    static constexpr float logo = 2000;

    void updateRainbow(GameRenderer &);
    void updateGamee(GameRenderer &);
    void updateBanner(GameRenderer &);

public:
    void update(delta_type, void *);

private:
    delta_type elapsed{0};
};


}


#endif // FACE_SMASH_PROCESS_PROCESS_SPLASH_H
