#ifndef FACE_SMASH_SERVICE_BILLING_NULL_H
#define FACE_SMASH_SERVICE_BILLING_NULL_H


#include "billing_service.h"


namespace gamee {


struct BillingNull : BillingService {
    void performPurchase(Product) noexcept override;
    void queryPurchase(Product) const noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_BILLING_NULL_H
