#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../service/billing_null.h"
#include "../service/ads_null.h"
#include "billing_system.h"


namespace gamee {


BillingSystem::BillingSystem()
    : isFaceSmashSupporter{false}
{
    Locator::Dispatcher::ref().sink<BillingEvent>().connect(this);
}


BillingSystem::~BillingSystem() {
    Locator::Dispatcher::ref().sink<BillingEvent>().disconnect(this);
}


void BillingSystem::receive(const BillingEvent &event) noexcept {
    if(event.product == Product::REMOVE_ADS) {
        // if you are a face smash supporter, you'll be a face smash supporter forever (yay!)
        isFaceSmashSupporter = isFaceSmashSupporter || ((event.type == BillingEvent::Type::PURCHASE_OK) || (event.type == BillingEvent::Type::ALREADY_PURCHASED));
    }
}


void BillingSystem::update(Registry &registry) {
    // we only add it and never remove the face smash supporter tag (yay!)
    if(isFaceSmashSupporter && registry.empty<FaceSmashSupporter>()) {
        registry.assign<FaceSmashSupporter>(registry.create());
        // stop and disable services that are no longer required
        Locator::Billing::set<BillingNull>();
        Locator::Ads::set<AdsNull>();
    }
}


}
