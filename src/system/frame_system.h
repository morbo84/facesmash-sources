#ifndef FACE_SMASH_SYSTEM_FRAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_FRAME_SYSTEM_H


namespace gamee {


struct FrameEvent;


struct FrameSystem final {
    FrameSystem();
    ~FrameSystem();

    void receive(const FrameEvent &) noexcept;

    void update();

private:
    bool pending;
};


}


#endif // FACE_SMASH_SYSTEM_FRAME_SYSTEM_H
