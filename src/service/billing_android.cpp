#include "billing_android.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include <string>


namespace gamee {


#ifdef __ANDROID__
extern void initiatePurchaseFlow(int);
extern void queryPurchases();
extern bool dequeuePurchaseUpdates(std::pair<int, int> &);
#else
static void initiatePurchaseFlow(int) {}
static void queryPurchases() {}
static bool dequeuePurchaseUpdates(std::pair<int, int> &) { return {}; }
#endif


void BillingAndroid::performPurchase(Product p) noexcept {
    initiatePurchaseFlow(static_cast<int>(p));
}


void BillingAndroid::queryPurchases() const noexcept {
    gamee::queryPurchases();
}


void BillingAndroid::dequeue() noexcept {
    std::pair<int, int> p;

    while(dequeuePurchaseUpdates(p)) {
        BillingEvent event{static_cast<Product>(p.first), static_cast<BillingEvent::Type >(p.second)};
        Locator::Dispatcher::ref().enqueue<BillingEvent>(event);
    }
}


}
