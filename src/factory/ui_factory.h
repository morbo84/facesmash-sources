#ifndef FACE_SMASH_FACTORY_UI_FACTORY_H
#define FACE_SMASH_FACTORY_UI_FACTORY_H


#include "../common/types.h"


namespace gamee {


void setPos(Registry &, entity_type, float, float);
void setSpriteSize(Registry &, entity_type, int, int);
void setBoundingBox(Registry &, entity_type, int, int, int, int);

entity_type createPanel(Registry &, PanelType, float, float, int, int);
entity_type createSprite(Registry &, entity_type, SDLTextureHandle, int);
entity_type createUIButton(Registry &, entity_type, SDLTextureHandle, UIAction, int);
entity_type createSmashButton(Registry &, entity_type, SDLTextureHandle, FaceType, int);

entity_type createHUD(Registry &, entity_type, SDLTextureHandle, int);
entity_type createHUD(Registry &, SDLTextureHandle, int);



}


#endif // FACE_SMASH_FACTORY_UI_FACTORY_H
