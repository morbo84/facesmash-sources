#ifndef FACE_SMASH_SYSTEM_EASTER_EGG_SYSTEM_H
#define FACE_SMASH_SYSTEM_EASTER_EGG_SYSTEM_H


#include "../common/bag.h"
#include "../common/types.h"


namespace gamee {


struct PlayFactory;
struct ActivateEasterEggEvent;


struct EasterEggSystem final {
    EasterEggSystem();
    ~EasterEggSystem();

    void receive(const ActivateEasterEggEvent &) noexcept;

    void update(Registry &, PlayFactory &);

private:
    bool dirty{false};
    int unlock{5};
    FaceBag bag;
};


}


#endif // FACE_SMASH_SYSTEM_EASTER_EGG_SYSTEM_H
