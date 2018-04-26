#ifndef FACE_SMASH_SERVICE_BILLING_SERVICE_H
#define FACE_SMASH_SERVICE_BILLING_SERVICE_H


#include "../common/types.h"


namespace gamee {


struct BillingService {
    virtual ~BillingService() = default;
    virtual void performPurchase(Product) noexcept = 0;
    virtual void queryPurchases() const noexcept = 0;
    virtual void dequeue() noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_BILLING_SERVICE_H
