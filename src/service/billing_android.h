#ifndef FACE_SMASH_SERVICE_BILLING_ANDROID_H
#define FACE_SMASH_SERVICE_BILLING_ANDROID_H


#include "billing_service.h"


namespace gamee {


struct BillingAndroid : BillingService {
    void performPurchase(Product) noexcept override;
    void queryPurchases() const noexcept override;
    void dequeue() noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_BILLING_ANDROID_H
