#ifndef FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCENE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class SceneSystem final {
    void backgroundFrame(Registry &);
    void gameUI(Registry &);
    void camera(Registry &);
    void spawner(Registry &);

#if DEBUG
    void smashButtons(Registry &);
    void debug(Registry &);
#endif // DEBUG

    void splashScreen(Registry &);
    void mainMenu(Registry &);
    void theGame(Registry &);
    void gameOver(Registry &);

public:
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
