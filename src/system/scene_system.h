#ifndef FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCENE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SceneChangeEvent;


class SceneSystem final {
    void discardSplashScreen(Registry &);
    void disableUIControls(Registry &);
    void enableUIControls(Registry &);
    void disableCameraFrame(Registry &);
    void enableCameraFrame(Registry &);
    void resetGame(Registry &);
    void resetTutorial(Registry &);

    delta_type menuPageTransition(Registry &, delta_type);
    delta_type gameTutorialTransition(Registry &);
    delta_type theGameTransition(Registry &);
    delta_type gameOverTransition(Registry &);
    delta_type trainingTutorialTransition(Registry &);
    delta_type trainingTransition(Registry &);

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
