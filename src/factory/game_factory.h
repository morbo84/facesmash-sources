#ifndef FACE_SMASH_FACTORY_GAME_FACTORY_H
#define FACE_SMASH_FACTORY_GAME_FACTORY_H


#include "../common/types.h"


namespace gamee {


void createSplashScreen(Registry &);
void createBackgroundTopPanel(Registry &);
void createBackgroundBottomPanel(Registry &);
void createMenuTopPanel(Registry &);
void createMenuBottomPanel(Registry &);
void createCreditsPanel(Registry &);
void createTutorialTopPanel(Registry &);
void createTutorialBottomPanel(Registry &);
void createGameTopPanel(Registry &);
void createGameBottomPanel(Registry &);
void createGameOverPanel(Registry &);
void createTrainingTopPanel(Registry &);
void createTrainingBottomPanel(Registry &);
void createCamera(Registry &);


#ifdef CAMERA_FRAME_AVAILABLE
void createCameraFrame(Registry &);
#endif // CAMERA_FRAME_AVAILABLE


#ifdef DEBUG
void createSmashButtons(Registry &);
void createDebugHUD(Registry &);
#endif // DEBUG


}


#endif // FACE_SMASH_FACTORY_GAME_FACTORY_H
