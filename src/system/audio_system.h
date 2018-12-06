#ifndef FACE_SMASH_SYSTEM_AUDIO_SYSTEM_H
#define FACE_SMASH_SYSTEM_AUDIO_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct AudioEvent;
struct AudioMusicEvent;


class AudioSystem final {
    static constexpr auto fadeIn = 500;
    static constexpr auto fadeOut = 750;

    void start() noexcept;
    void stop() noexcept;

public:
    AudioSystem();
    ~AudioSystem();

    void onAudioEvent(const AudioEvent &) noexcept;
    void onAudioMusicEvent(const AudioMusicEvent &) noexcept;

    void update();

private:
    AudioMusicType curr;
    AudioMusicType next;
};


}


#endif // FACE_SMASH_SYSTEM_AUDIO_SYSTEM_H
