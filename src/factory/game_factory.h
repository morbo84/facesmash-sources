#ifndef FACE_SMASH_FACTORY_GAME_FACTORY_H
#define FACE_SMASH_FACTORY_GAME_FACTORY_H


#include "../common/types.h"


namespace gamee {


void createExitPanel(Registry &);
void createSplashScreenPanel(Registry &);
void createBackgroundTopPanel(Registry &);
void createBackgroundBottomPanel(Registry &);
void createMenuTopPanel(Registry &);
void createMenuBottomPanel(Registry &);
void createCreditsPanel(Registry &);
void createSupportPanel(Registry &);
void refreshSupportPanel(Registry &);
void createSettingsPanel(Registry &);
void createTutorialTopPanel(Registry &);
void createTutorialBottomPanel(Registry &);
void createGameTopPanel(Registry &);
void createGameBottomPanel(Registry &);
void createGameOverPanel(Registry &);
void refreshGameOverPanel(Registry &);
void createTrainingTopPanel(Registry &);
void createTrainingBottomPanel(Registry &);
void createCamera(Registry &);
void createCameraFrame(Registry &);
void createSmashButtons(Registry &);
void createDebugHUD(Registry &);


}


#endif // FACE_SMASH_FACTORY_GAME_FACTORY_H
