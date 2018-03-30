#ifndef FACE_SMASH_SYSTEM_FRAME_SYSTEM_H
#define FACE_SMASH_SYSTEM_FRAME_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct FrameAvailableEvent;
struct PermissionEvent;
struct GameRenderer;


struct FrameSystem final {
    FrameSystem() noexcept;
    ~FrameSystem() noexcept;

    void receive(const FrameAvailableEvent &) noexcept;
    void receive(const PermissionEvent &) noexcept;

    void update(Registry &, GameRenderer &);

private:
    bool refresh;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_FRAME_SYSTEM_H
