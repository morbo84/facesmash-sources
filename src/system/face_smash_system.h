#ifndef FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"
#include "../resource/texture_resource.h"


namespace gamee {


struct FaceEvent;
struct BonusEvent;
struct Spawner;


struct FaceSmashSystem final {
    FaceSmashSystem();
    ~FaceSmashSystem();

    void receive(const FaceEvent &) noexcept;
    void receive(const BonusEvent &) noexcept;

    void update(Registry &, Spawner &);

private:
    FaceType type;
    bool dirty;
    bool smashAll;
    bool richMan;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
