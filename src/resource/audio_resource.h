#ifndef FACE_SMASH_RESOURCE_AUDIO_RESOURCE_H
#define FACE_SMASH_RESOURCE_AUDIO_RESOURCE_H


#include <memory>
#include <SDL_mixer.h>
#include <entt/resource/handle.hpp>
#include <entt/resource/loader.hpp>


namespace gamee {


struct AudioMusicResource final {
    explicit AudioMusicResource(Mix_Music *) noexcept;

    AudioMusicResource(const AudioMusicResource &) = delete;
    AudioMusicResource(AudioMusicResource &&) = delete;

    ~AudioMusicResource() noexcept;

    AudioMusicResource & operator=(const AudioMusicResource &) = delete;
    AudioMusicResource & operator=(AudioMusicResource &&) = delete;

    operator Mix_Music *() const noexcept;

private:
    Mix_Music *music;
};


struct AudioChunkResource final {
    explicit AudioChunkResource(Mix_Chunk *) noexcept;

    AudioChunkResource(const AudioChunkResource &) = delete;
    AudioChunkResource(AudioChunkResource &&) = delete;

    ~AudioChunkResource() noexcept;

    AudioChunkResource & operator=(const AudioChunkResource &) = delete;
    AudioChunkResource & operator=(AudioChunkResource &&) = delete;

    operator Mix_Chunk *() const noexcept;

private:
    Mix_Chunk *chunk;
};


struct AudioMusicLoader final: public entt::resource_loader<AudioMusicLoader, AudioMusicResource> {
    std::shared_ptr<AudioMusicResource> load(const char *) const;
};


struct AudioChunkLoader final: public entt::resource_loader<AudioChunkLoader, AudioChunkResource> {
    std::shared_ptr<AudioChunkResource> load(const char *) const;
};


using AudioMusicHandle = entt::resource_handle<AudioMusicResource>;
using AudioChunkHandle = entt::resource_handle<AudioChunkResource>;


}


#endif // FACE_SMASH_RESOURCE_AUDIO_RESOURCE_H
