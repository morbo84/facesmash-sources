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

    void onFrameAvailable(const FrameAvailableEvent &) noexcept;
    void onPermission(const PermissionEvent &) noexcept;

    void update(Registry &, GameRenderer &);

private:
    bool refresh;
    bool dirty;
};


}


#endif // FACE_SMASH_SYSTEM_FRAME_SYSTEM_H
