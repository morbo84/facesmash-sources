#ifndef FACE_SMASH_FACTORY_GAME_FACTORY_H
#define FACE_SMASH_FACTORY_GAME_FACTORY_H


#include "../common/types.h"


namespace gamee {


void createExitPanel(Registry &);
void createTitleTopPanel(Registry &);
void createTitleBottomPanel(Registry &);
void createSplashScreenPanel(Registry &);
void createBackgroundTopPanel(Registry &);
void createBackgroundBottomPanel(Registry &);
void createMenuTopPanel(Registry &);
void createMenuBottomPanel(Registry &);
void createInfoPanel(Registry &);
void refreshInfoPanel(Registry &);
void createMultiplayerPanel(Registry &);
void createSettingsPanel(Registry &);
void createTutorialTopPanel(Registry &);
void createTutorialBottomPanel(Registry &);
void createGameTopPanel(Registry &);
void createGameBottomPanel(Registry &);
void createGameOverPanel(Registry &);
void refreshGameOverPanel(Registry &);
void createMultiplayerResultsPanel(Registry &);
void refreshResultsMultiplayerResultsPanel(Registry &);
void refreshButtonsMultiplayerResultsPanel(Registry &);
void createTrainingRightPanel(Registry &);
void createTrainingTopPanel(Registry &);
void createTrainingBottomPanel(Registry &);
void createInviteTrainLeft(Registry &);
void createInviteTrainRight(Registry &);
void createInviteShareLeft(Registry &);
void createInviteShareRight(Registry &);
void createStanza(Registry &);
void createCamera(Registry &);
void createCameraFrame(Registry &);
void createSmashButtons(Registry &);
void createDebugHUD(Registry &);


}


#endif // FACE_SMASH_FACTORY_GAME_FACTORY_H
