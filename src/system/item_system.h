#ifndef FACE_SMASH_SYSTEM_ITEM_SYSTEM_H
#define FACE_SMASH_SYSTEM_ITEM_SYSTEM_H


#include <SDL_rect.h>
#include "../common/bag.h"
#include "../common/types.h"


namespace gamee {


struct TouchEvent;
struct ArmageddonEvent;
struct Spawner;


class ItemSystem final {
    void movement(Registry &, float);
    void fountain(Registry &, Spawner &);

    void message(Registry &, SDLTextureHandle);
    void message(Registry &, ItemType);

public:
    ItemSystem();
    ~ItemSystem();

    delta_type toRemaining(ItemType);

    void receive(const TouchEvent &) noexcept;
    void receive(const ArmageddonEvent &) noexcept;

    void update(Registry &, Spawner &, delta_type);

private:
    SDL_Point coord;
    bool dirty;
    bool armageddon;
    ItemType curr;
    delta_type remaining;
    FaceBag faceBag;
    ItemBag itemBag;
};


}


#endif // FACE_SMASH_SYSTEM_ITEM_SYSTEM_H
