#ifndef FACE_SMASH_SYSTEM_FRAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_FRAME_SYSTEM_H


namespace gamee {

class FrameAvailableEvent;

struct FrameSystem final {
    FrameSystem() noexcept;
    ~FrameSystem() noexcept;
    void update();
    void receive(const FrameAvailableEvent &) noexcept;

private:
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_FRAME_SYSTEM_H
