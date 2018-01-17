#ifndef FACE_SMASH_SYSTEM_ITEM_SYSTEM_H
#define FACE_SMASH_SYSTEM_ITEM_SYSTEM_H


#include <SDL_rect.h>
#include "../common/bag.h"
#include "../common/types.h"


namespace gamee {


struct TouchEvent;
struct PlayFactory;


class ItemSystem final {
    void movement(Registry &, float);
    void fountain(Registry &, PlayFactory &);

public:
    ItemSystem();
    ~ItemSystem();

    delta_type toRemaining(ItemType);

    void receive(const TouchEvent &) noexcept;

    void update(Registry &, PlayFactory &, delta_type);

private:
    SDL_Point coord;
    bool dirty;
    ItemType curr;
    delta_type remaining;
    FaceBag faceBag;
};


}


#endif // FACE_SMASH_SYSTEM_ITEM_SYSTEM_H
