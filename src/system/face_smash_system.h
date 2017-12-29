#ifndef FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H


#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct FaceSmashEvent;


class FaceSmashSystem final {
    void addScore(Registry &, SDLTextureHandle, SDL_Point);

public:
    FaceSmashSystem();
    ~FaceSmashSystem();

    void receive(const FaceSmashEvent &) noexcept;

    void update(Registry &);

private:
    SmashType type;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
