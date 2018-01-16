#ifndef FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H
#define FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H


namespace gamee {


struct AvMuxerService {
    virtual ~AvMuxerService() = default;

    virtual bool isRecording() const noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_AV_MUXER_SERVICE_H
