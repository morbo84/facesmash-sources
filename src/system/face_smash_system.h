#ifndef FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H


#include "../types/types.hpp"


namespace gamee {


struct SmashEvent;


struct FaceSmashSystem final {
    FaceSmashSystem();
    ~FaceSmashSystem();

    void receive(const SmashEvent &) noexcept;

    void update(Registry &, delta_type);

private:
    SmashType type;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_SMASH_SYSTEM_H
