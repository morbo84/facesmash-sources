#ifndef FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
#define FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H


#include "../common/types.h"
#include "../component/component.hpp"


namespace gamee {


struct SceneChangeEvent;
struct TimeIsOverEvent;
struct BillingEvent;


struct AchievementsSystem final {
    AchievementsSystem() noexcept;
    ~AchievementsSystem() noexcept;

    void onSceneChange(const SceneChangeEvent &) noexcept;
    void onTimeOver(const TimeIsOverEvent &) noexcept;
    void onBillingEvent(const BillingEvent &) noexcept;

    void update(Registry &);

private:
    bool thankYouSupporter;
    bool timeIsOver;
    SceneType current;
    PlayerScore previous{};
};


}


#endif // FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
