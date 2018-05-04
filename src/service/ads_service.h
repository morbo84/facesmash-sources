#ifndef FACE_SMASH_SERVICE_ADS_SERVICE_H
#define FACE_SMASH_SERVICE_ADS_SERVICE_H


namespace gamee {


enum class AdsType {
    BANNER,
    INTERSTITIAL
};


struct AdsService {
    virtual ~AdsService() = default;

    virtual void load(AdsType) = 0;
    virtual bool isLoaded(AdsType) = 0;
    virtual void show(AdsType) = 0;
};


}


#endif // FACE_SMASH_SERVICE_ADS_SERVICE_H
