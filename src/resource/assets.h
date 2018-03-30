#ifndef FACE_SMASH_RESOURCE_ASSETS_H
#define FACE_SMASH_RESOURCE_ASSETS_H


namespace gamee {


struct GameRenderer;


void loadResources(GameRenderer &);
void loadDebugResources();

void prepareGameStrings(GameRenderer &);
void prepareDebugStrings(GameRenderer &);
void prepareCameraFrame(GameRenderer &);
void prepareGameStuff(GameRenderer &);


}


#endif // FACE_SMASH_RESOURCE_ASSETS_H
