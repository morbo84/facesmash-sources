#ifndef FACE_SMASH_RESOURCE_ASSETS_H
#define FACE_SMASH_RESOURCE_ASSETS_H


namespace gamee {


struct GameRenderer;


void loadDefaultFont();
void loadDebugFont();
void loadGameStuff(GameRenderer &);
void loadCameraFrame(GameRenderer &);
void loadDebugStuff(GameRenderer &);
void loadTargetTextures(GameRenderer &);


}


#endif // FACE_SMASH_RESOURCE_ASSETS_H
