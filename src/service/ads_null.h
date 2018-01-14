#ifndef FACE_SMASH_SERVICE_ADS_NULL_H
#define FACE_SMASH_SERVICE_ADS_NULL_H


#include "ads_service.h"


namespace gamee {


struct AdsNull : AdsService {
    void load(AdsType) override;
    void isLoaded(AdsType) override;
    void show(AdsType) override;
};


}


#endif // FACE_SMASH_SERVICE_ADS_NULL_H
