#ifndef FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCENE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SceneChangeEvent;
struct KeyboardEvent;
struct PermissionEvent;


struct SceneSystem final {
    SceneSystem();
    ~SceneSystem();

    void receive(const SceneChangeEvent &) noexcept;
    void receive(const KeyboardEvent &) noexcept;
    void receive(const PermissionEvent &) noexcept;

    void update(Registry &, delta_type);

public:
    SceneType curr;
    SceneType next;
    delta_type remaining;
    bool isTransitioning;
    bool forceRefreshGameOverPanel;
};


}


#endif // FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
