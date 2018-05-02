#ifndef FACE_SMASH_SYSTEM_BILLING_SYSTEM_H
#define FACE_SMASH_SYSTEM_BILLING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct BillingEvent;


struct BillingSystem final {
    BillingSystem();
    ~BillingSystem();

    void receive(const BillingEvent &) noexcept;

    void update(Registry &);

private:
    bool isFaceSmashSupporter;
};


}


#endif // FACE_SMASH_SYSTEM_BILLING_SYSTEM_H
