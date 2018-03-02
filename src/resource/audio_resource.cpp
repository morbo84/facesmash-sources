#include <cassert>
#include "audio_resource.h"


namespace gamee {


AudioMusicResource::AudioMusicResource(Mix_Music *music) noexcept
    : music{music}
{
    assert(music);
}


AudioMusicResource::~AudioMusicResource() noexcept {
    Mix_FreeMusic(music);
}


AudioMusicResource::operator Mix_Music *() const noexcept {
    return music;
}


AudioChunkResource::AudioChunkResource(Mix_Chunk *chunk) noexcept
    : chunk{chunk}
{
    assert(chunk);
}


AudioChunkResource::~AudioChunkResource() noexcept {
    Mix_FreeChunk(chunk);
}


AudioChunkResource::operator Mix_Chunk *() const noexcept {
    return chunk;
}


std::shared_ptr<AudioMusicResource> AudioMusicLoader::load(const char *res) const {
    std::shared_ptr<AudioMusicResource> ret;
    Mix_Music *music = Mix_LoadMUS(res);

    if(music) {
        ret = std::make_shared<AudioMusicResource>(music);
    }

    return ret;
}


std::shared_ptr<AudioChunkResource> AudioChunkLoader::load(const char *res) const {
    std::shared_ptr<AudioChunkResource> ret;
    Mix_Chunk *chunk = Mix_LoadWAV(res);

    if(chunk) {
        ret = std::make_shared<AudioChunkResource>(chunk);
    }

    return ret;
}


}
