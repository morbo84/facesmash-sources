#include "billing_null.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"


namespace gamee {


void BillingNull::performPurchase(Product p) noexcept {
    Locator::Dispatcher::ref().enqueue<BillingEvent>(p, BillingEvent::Type::PURCHASE_ERROR);
}


void BillingNull::queryPurchases() const noexcept {
    Locator::Dispatcher::ref().enqueue<BillingEvent>(Product::REMOVE_ADS, BillingEvent::Type::NOT_PURCHASED);
}


void BillingNull::dequeue() noexcept {}


}
