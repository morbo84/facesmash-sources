#ifndef FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCENE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SceneChangeEvent;
struct KeyboardEvent;


class SceneSystem final {
    void discardExpiringContents(Registry &);
    void discardSplashScreen(Registry &);
    void disableUIControls(Registry &);
    void enableUIControls(Registry &);
    void disableCameraFrame(Registry &);
    void enableCameraFrame(Registry &);
    void resetGame(Registry &);
    void resetTraining(Registry &);

    delta_type splashScreenTransition(Registry &);
    delta_type menuPageTransition(Registry &);
    delta_type creditsTransition(Registry &);
    delta_type supportTransition(Registry &);
    delta_type settingsTransition(Registry &);
    delta_type achievementsTransition(Registry &);
    delta_type gameTutorialTransition(Registry &);
    delta_type theGameTransition(Registry &);
    delta_type gameOverTransition(Registry &);
    delta_type trainingTutorialTransition(Registry &);
    delta_type trainingTransition(Registry &);

public:
    SceneSystem();
    ~SceneSystem();

    void receive(const SceneChangeEvent &) noexcept;
    void receive(const KeyboardEvent &) noexcept;

    void update(Registry &, delta_type);

public:
    SceneType curr;
    SceneType next;
    delta_type remaining;
    bool isTransitioning;
};


}


#endif // FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
