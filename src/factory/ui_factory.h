#ifndef FACE_SMASH_FACTORY_UI_FACTORY_H
#define FACE_SMASH_FACTORY_UI_FACTORY_H


#include "../common/types.h"


namespace gamee {


entity_type createPanel(Registry &, PanelType, float, float, int, int);
entity_type createUIButton(Registry &, entity_type, UIAction, int);
entity_type createPopupUIButton(Registry &, entity_type, UIAction, int);
entity_type createHUD(Registry &, entity_type, SDLTextureHandle, int);
entity_type createHUD(Registry &, SDLTextureHandle, int);
entity_type createBoxBorder(Registry &, entity_type, BoxBorderType, int, int);
entity_type createBoxBorder(Registry &, entity_type, BoxBorderType, int, int);
entity_type createProgressBar(Registry &, entity_type, int, int);

}


#endif // FACE_SMASH_FACTORY_UI_FACTORY_H
