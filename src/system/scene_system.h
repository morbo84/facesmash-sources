#ifndef FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCENE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SceneChangeEvent;


class SceneSystem final {
    void discardSplashScreen(Registry &);
    void disableUIControls(Registry &);
    void enableUIControls(Registry &);

    delta_type menuPageTransition(Registry &, delta_type);
    delta_type theGameTransition(Registry &);
    delta_type gameOverTransition(Registry &);

    void splashScreen();
    void menuPage(Registry &);
    void theGame(Registry &);
    void gameOver(Registry &);

public:
    SceneSystem();
    ~SceneSystem();

    void receive(const SceneChangeEvent &) noexcept;

    void update(Registry &, delta_type);

public:
    SceneType curr;
    SceneType next;
    delta_type remaining;
    bool isTransitioning;
};


}


#endif // FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
