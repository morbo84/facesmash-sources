#ifndef FACE_SMASH_RESOURCE_FRAME_RESOURCE_H
#define FACE_SMASH_RESOURCE_FRAME_RESOURCE_H


struct SDL_Renderer;
struct SDL_Texture;


namespace gamee {


struct FrameTexture final {
    explicit FrameTexture(SDL_Renderer *, int, int) noexcept;

    FrameTexture(const FrameTexture &) = delete;
    FrameTexture(FrameTexture &&) = delete;

    ~FrameTexture() noexcept;

    FrameTexture & operator=(const FrameTexture &) = delete;
    FrameTexture & operator=(FrameTexture &&) = delete;

    operator SDL_Texture *() const noexcept;

    int width() const noexcept;
    int height() const noexcept;

private:
    SDL_Texture *texture;
};


}


#endif // FACE_SMASH_RESOURCE_FRAME_RESOURCE_H
