#ifndef FACE_SMASH_SERVICE_ADS_ANDROID_H
#define FACE_SMASH_SERVICE_ADS_ANDROID_H


#include "ads_service.h"


namespace gamee {


struct AdsAndroid : AdsService {
    void load(AdsType) override;
    bool isLoaded(AdsType) override;
    void show(AdsType) override;
};


}


#endif // FACE_SMASH_SERVICE_ADS_ANDROID_H
