#include "billing_android.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include <string>


namespace gamee {


void initiatePurchaseFlow(int purchaseId);
void queryPurchases();
bool dequeuePurchaseUpdates(std::pair<int, int>& p);


void BillingAndroid::performPurchase(Product p) noexcept {
    initiatePurchaseFlow(static_cast<int>(p));
}


void BillingAndroid::queryPurchases() const noexcept {
    queryPurchases();
}


void BillingAndroid::dequeue() noexcept {
    std::pair<int, int> p;
    while(dequeuePurchaseUpdates(p)) {
        BillingEvent event{static_cast<Product>(p.first), static_cast<BillingEvent::Type >(p.second)};
        Locator::Dispatcher::ref().enqueue<BillingEvent>(event);
    }
}


}
