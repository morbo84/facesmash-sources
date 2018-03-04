#ifndef FACE_SMASH_SYSTEM_AUDIO_SYSTEM_H
#define FACE_SMASH_SYSTEM_AUDIO_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SceneChangeEvent;
struct SmashEvent;


class AudioSystem final {
    static constexpr delta_type interval = 100_ui32;
    static constexpr int fade = 250_ui32;

public:
    AudioSystem();
    ~AudioSystem();

    void receive(const SceneChangeEvent &) noexcept;
    void receive(const SmashEvent &) noexcept;
    void receive(const EnvEvent &) noexcept;

    void update(Registry &, delta_type);

private:
    delta_type remaining;
    unsigned int explosion;
    SceneType curr;
    SceneType next;
};


}


#endif // FACE_SMASH_SYSTEM_AUDIO_SYSTEM_H
