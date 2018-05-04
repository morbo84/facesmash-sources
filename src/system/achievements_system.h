#ifndef FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
#define FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H


#include "../common/types.h"
#include "../component/component.hpp"


namespace gamee {


struct SceneChangeEvent;
struct BillingEvent;


struct AchievementsSystem final {
    AchievementsSystem() noexcept;
    ~AchievementsSystem() noexcept;

    void receive(const SceneChangeEvent &) noexcept;
    void receive(const BillingEvent &) noexcept;

    void update(Registry &);

private:
    bool thankYouSupporter;
    bool dirtyGameOver;
    SceneType current;
    PlayerScore previous{};
};


}


#endif // FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
