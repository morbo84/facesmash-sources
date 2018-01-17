#ifndef FACE_SMASH_RESOURCE_ASSETS_H
#define FACE_SMASH_RESOURCE_ASSETS_H


namespace gamee {


struct GameRenderer;


void loadDefaultFont(GameRenderer &);
void loadGameStuff(GameRenderer &);


#ifdef CAMERA_FRAME_AVAILABLE
void loadCameraFrame(GameRenderer &);
#endif // CAMERA_FRAME_AVAILABLE


#if DEBUG
void loadDebugFont(GameRenderer &);
#endif // DEBUG


}


#endif // FACE_SMASH_RESOURCE_ASSETS_H
