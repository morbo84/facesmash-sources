#ifndef FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
#define FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H


#include "../event/event.hpp"
#include "../common/types.h"


namespace gamee {


struct AchievementsSystem final {
    AchievementsSystem() noexcept;
    ~AchievementsSystem() noexcept;

    void receive(const SceneChangeEvent &) noexcept;
    void receive(const SmashEvent &) noexcept;

    void faceSmashSupporter(Registry &);
    void myFirstCombo(Registry &);
    void collector2x(Registry &);
    void collector3x(Registry &);
    void collector4x(Registry &);
    void collector5x(Registry &);
    void smashMeBaby(Registry &);
    void theSniper(Registry &);
    void godSmasher(Registry &);
    void angrySmash(Registry &);
    void disgustedSmash(Registry &);
    void fearfulSmash(Registry &);
    void happySmash(Registry &);
    void sadSmash(Registry &);
    void surprisedSmash(Registry &);
    void midnightCombo(Registry &);
    void noonCombo(Registry &);
    void iAmSoHappy(Registry &);
    void blueIsTheNewSmash(Registry &);
    void noPainNoGame(Registry &);
    void allInARow(Registry &);
    void kindergarten(Registry &);
    void readyToSmash(Registry &);
    void smashIsMyJob(Registry &);

    void update(Registry &);

private:
    bool dirtySmashEvent;
    bool dirtyGameOver;
    SceneChangeEvent gameOverEvent;
    SmashEvent smashEvent;
    SceneType current;
};


}


#endif // FACE_SMASH_SYSTEM_ACHIEVEMENTS_SYSTEM_H
