#include "billing_null.h"
#include "../event/event.hpp"
#include "../locator/locator.hpp"


namespace gamee {


void BillingNull::performPurchase(Product p) noexcept {
    Locator::Dispatcher::ref().enqueue<BillingEvent>(p, BillingEvent::Type::PURCHASE_ERROR);
}


void BillingNull::queryPurchase(Product p) const noexcept {
    Locator::Dispatcher::ref().enqueue<BillingEvent>(p, BillingEvent::Type::NOT_PURCHASED);
}


}
