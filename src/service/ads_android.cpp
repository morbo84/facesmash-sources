#include "ads_android.h"


namespace gamee {


#ifdef __ANDROID__
void bindingLoadInterstitialAd();
bool bindingIsLoadedIntestitialAd();
void bindingShowInterstitialAd();
void bindingLoadBannerAd();
bool bindingIsLoadedBannerAd();
void bindingShowBannerAd();
#else
void bindingLoadInterstitialAd() {}
bool bindingIsLoadedIntestitialAd() { return false; }
void bindingShowInterstitialAd() {}
void bindingLoadBannerAd() {}
bool bindingIsLoadedBannerAd() { return false; }
void bindingShowBannerAd() {}
#endif


void AdsAndroid::load(AdsType type) {
    switch(type) {
    case AdsType::BANNER:
        bindingLoadBannerAd();
        break;
    case AdsType::INTERSTITIAL:
        bindingLoadInterstitialAd();
        break;
    }
}


bool AdsAndroid::isLoaded(AdsType type) {
    bool ret{false};

    switch(type) {
    case AdsType::BANNER:
        ret = bindingIsLoadedBannerAd();
        break;
    case AdsType::INTERSTITIAL:
        ret = bindingIsLoadedIntestitialAd();
        break;
    }

    return ret;
}


void AdsAndroid::show(AdsType type) {
    switch(type) {
    case AdsType::BANNER:
        bindingShowBannerAd();
        break;
    case AdsType::INTERSTITIAL:
        bindingShowInterstitialAd();
        break;
    }
}


void AdsAndroid::stop() {
    // TODO
}


}
