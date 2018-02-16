#ifndef FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
#define FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H


#include "../common/types.h"


namespace gamee {


struct SceneChangeEvent;


struct AchievementsSystem final {
    AchievementsSystem() noexcept;
    ~AchievementsSystem() noexcept;

    void receive(const SceneChangeEvent &) noexcept;

    void update(Registry &);

private:
    bool dirtyGameOver;
    SceneType current;
};


}


#endif // FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
