#ifndef FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct FaceSmashEvent;


struct FaceSmashSystem final {
    FaceSmashSystem();
    ~FaceSmashSystem();

    void receive(const FaceSmashEvent &) noexcept;

    void update(Registry &, delta_type);

private:
    SmashType type;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
