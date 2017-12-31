#ifndef FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCENE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class SceneSystem final {
    void splashScreen(Registry &);
    void mainMenu(Registry &);
    void training(Registry &);
    void timer(Registry &);
    void challenge(Registry &);

    void backgroundFrame(Registry &);
    void hudScore(Registry &);
    void camera(Registry &);

#if DEBUG
    void smashButtons(Registry &);
    void debug(Registry &);
#endif // DEBUG

public:
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
