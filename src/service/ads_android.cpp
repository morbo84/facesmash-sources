#include "ads_android.h"


namespace gamee {


#ifdef __ANDROID__
void bindingLoadIntestitialAd();
bool bindingIsLoadedIntestitialAd();
void bindingShowInterstitialAd();
void bindingLoadBannerAd();
bool bindingIsLoadedBannerAd();
void bindingShowBannerAd();
#else
void bindingLoadIntestitialAd() {}
bool bindingIsLoadedIntestitialAd() { return false; }
void bindingShowInterstitialAd() {}
void bindingLoadBannerAd() {}
bool bindingIsLoadedBannerAd() { return false; }
void bindingShowBannerAd() {}
#endif


void AdsAndroid::load(AdsType t) {
    switch (t) {
    case AdsType::BANNER:
        bindingLoadBannerAd();
        break;
    case AdsType::INTERSTITIAL:
        bindingLoadIntestitialAd();
        break;
    }
}


bool AdsAndroid::isLoaded(AdsType t) {
    bool ret{false};
    switch (t) {
    case AdsType::BANNER:
        ret = bindingIsLoadedBannerAd();
        break;
    case AdsType::INTERSTITIAL:
        ret = bindingIsLoadedIntestitialAd();
        break;
    }

    return ret;
}


void AdsAndroid::show(AdsType t) {
    switch (t) {
    case AdsType::BANNER:
        bindingShowBannerAd();
        break;
    case AdsType::INTERSTITIAL:
        bindingShowInterstitialAd();
        break;
    }
}


}
