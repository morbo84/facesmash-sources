#ifndef FACE_SMASH_SERVICE_AUDIO_SERVICE_H
#define FACE_SMASH_SERVICE_AUDIO_SERVICE_H


namespace gamee {


struct AudioService {
    virtual ~AudioService() = default;

    virtual bool isMute() const noexcept = 0;

    // TODO
};


}


#endif // FACE_SMASH_SERVICE_AUDIO_SERVICE_H
