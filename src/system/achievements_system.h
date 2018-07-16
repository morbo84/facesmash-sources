#ifndef FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
#define FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H


#include "../common/types.h"
#include "../component/component.hpp"


namespace gamee {


struct AchievementEvent;
struct SceneChangeEvent;
struct TimeIsOverEvent;
struct BillingEvent;


struct AchievementsSystem final {
    AchievementsSystem() noexcept;
    ~AchievementsSystem() noexcept;

    void receive(const AchievementEvent &) noexcept;
    void receive(const SceneChangeEvent &) noexcept;
    void receive(const TimeIsOverEvent &) noexcept;
    void receive(const BillingEvent &) noexcept;

    void update(Registry &);

private:
    bool thankYouSupporter;
    bool littleSmasherUnlocked;
    bool theSniperUnlocked;
    bool timeIsOver;
    SceneType current;
    PlayerScore previous{};
};


}


#endif // FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
