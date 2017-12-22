#ifndef FACE_SMASH_INPUT_INPUT_HANDLER_H
#define FACE_SMASH_INPUT_INPUT_HANDLER_H


namespace gamee {


struct InputHandler {
    virtual ~InputHandler() noexcept = default;

    virtual void poll() noexcept = 0;
    virtual void wait() noexcept = 0;
    virtual void waitFor(int) noexcept = 0;
};


}


#endif // FACE_SMASH_INPUT_INPUT_HANDLER_H
