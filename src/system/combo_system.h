#ifndef FACE_SMASH_SYSTEM_COMBO_SYSTEM_H
#define FACE_SMASH_SYSTEM_COMBO_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SmashEvent;


class ComboSystem final {
    static constexpr delta_type interval = 500;
    static constexpr UInt8 max = 5;

public:
    ComboSystem();
    ~ComboSystem();

    void receive(const SmashEvent &) noexcept;

    void update(Registry &, delta_type);

private:
    delta_type remaining;
    UInt8 current;
};


}


#endif // FACE_SMASH_SYSTEM_COMBO_SYSTEM_H
