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

    void onSceneChange(const SceneChangeEvent &) noexcept;
    void onKeyboardEvent(const KeyboardEvent &) noexcept;
    void onPermission(const PermissionEvent &) noexcept;

    void update(Registry &, delta_type);

public:
    SceneType curr;
    SceneType next;
    SceneType pending;
    delta_type remaining;
    bool isTransitioning;
};


}


#endif // FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
