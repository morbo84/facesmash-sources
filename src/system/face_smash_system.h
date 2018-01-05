#ifndef FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"
#include "../resource/texture_resource.h"


namespace gamee {


struct FaceEvent;


class FaceSmashSystem final {
    void addScore(Registry &, SDLTextureHandle, SDL_Point);

public:
    FaceSmashSystem();
    ~FaceSmashSystem();

    void receive(const FaceEvent &) noexcept;

    void update(Registry &);

private:
    FaceType type;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
