#ifndef FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
#define FACE_SMASH_SYSTEM_SCENE_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SceneChangeEvent;
struct KeyboardEvent;


struct SceneSystem final {
    SceneSystem();
    ~SceneSystem();

    void receive(const SceneChangeEvent &) noexcept;
    void receive(const KeyboardEvent &) noexcept;

    void update(Registry &, delta_type);

public:
    SceneType curr;
    SceneType next;
    delta_type remaining;
    bool isTransitioning;
    bool hasCameraPermission;
};


}


#endif // FACE_SMASH_SYSTEM_SCENE_SYSTEM_H
