#ifndef FACE_SMASH_SYSTEM_COMBO_SYSTEM_H
#define FACE_SMASH_SYSTEM_COMBO_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct FaceSmashEvent;
struct FaceMissEvent;


class ComboSystem final {
    static constexpr delta_type interval = 600;
    static constexpr delta_type bonus = 50;
    static constexpr UInt8 max = 5;

    void combo(Registry &, SDLTextureHandle, SDLTextureHandle, int);

public:
    ComboSystem();
    ~ComboSystem();

    void receive(const FaceSmashEvent &) noexcept;
    void receive(const FaceMissEvent &) noexcept;

    void update(Registry &, delta_type);

private:
    delta_type remaining;
    UInt8 current;
};


}


#endif // FACE_SMASH_SYSTEM_COMBO_SYSTEM_H
