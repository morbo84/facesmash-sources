#ifndef FACE_SMASH_SYSTEM_FACE_MODIFIER_SYSTEM_H
#define FACE_SMASH_SYSTEM_FACE_MODIFIER_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct FaceModifierEvent;


class FaceModifierSystem final {
    void update(Registry &, float);

public:
    FaceModifierSystem();
    ~FaceModifierSystem();

    void receive(const FaceModifierEvent &) noexcept;

    void update(Registry &, delta_type);

private:
    FaceModifier modifier;
    delta_type remaining;
};


}


#endif // FACE_SMASH_SYSTEM_FACE_MODIFIER_SYSTEM_H
