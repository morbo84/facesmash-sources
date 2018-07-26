#ifndef FACE_SMASH_SYSTEM_STANZA_SYSTEM_H
#define FACE_SMASH_SYSTEM_STANZA_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct Stanza;


class StanzaSystem final {
    static constexpr delta_type duration = 1500_ui32;

    void detectInGameItemTutorial(Registry &, Stanza &);
    void slowDownEverything(Registry &);

public:
    void update(Registry &, delta_type);

private:
    using action_fn_type = void(StanzaSystem::*)(Registry &);
    action_fn_type action_fn{nullptr};
    entity_type target{entt::null};
    delta_type remaining{0};
};


}


#endif // FACE_SMASH_SYSTEM_STANZA_SYSTEM_H
