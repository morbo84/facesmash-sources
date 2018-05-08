#ifndef FACE_SMASH_SERVICE_ACHIEVEMENTS_SERVICE_H
#define FACE_SMASH_SERVICE_ACHIEVEMENTS_SERVICE_H


#include <SDL_types.h>
#include <cstdint>


namespace gamee {


enum class FaceSmashAchievement {
    SMASH_ME_BABY,
    LITTLE_SMASHER,
    THE_SNIPER,
    GOD_SMASHER,
    MY_FIRST_COMBO,
    COMBO_COLLECTOR_2X,
    COMBO_COLLECTOR_3X,
    COMBO_COLLECTOR_4X,
    COMBO_COLLECTOR_5X,
    SMASH_THE_ANGRY,
    SMASH_THE_DISGUSTED,
    SMASH_THE_FEARFUL,
    SMASH_THE_HAPPY,
    SMASH_THE_SAD,
    SMASH_THE_SURPRISED,
    IM_SO_HAPPY,
    BLUE_SMASH,
    NO_PAIN_NO_GAME,
    KINDERGARTEN,
    READY_TO_SMASH,
    SMASH_IS_MY_JOB,
    OH_MY_SMASH,
    SMASH_ME_CRY,
    FACE_SMASH_SUPPORTER
};


enum class FaceSmashLeaderboard {
    SCORE, FACES
};


struct AchievementsManager {
    virtual ~AchievementsManager() = default;

    virtual void increment(FaceSmashAchievement a, uint32_t steps = 1) noexcept = 0;
    virtual void unlock(FaceSmashAchievement a) noexcept = 0;
    virtual void showAllUI() noexcept = 0;
};


struct LeaderboardsManager {
    virtual ~LeaderboardsManager() = default;

    virtual void submitScore(FaceSmashLeaderboard, uint64_t) noexcept = 0;
    virtual void showAllLeaderboardsUI() noexcept = 0;
};


struct GameServicesService {
    enum class Status: Uint8 {
        SIGNING_IN,
        SIGNED_IN,
        SIGNING_OUT,
        SIGNED_OUT
    };

    virtual ~GameServicesService() = default;

    virtual Status status() const noexcept = 0;
    virtual void signIn() noexcept = 0;
    virtual void signOut() noexcept = 0;
    virtual bool isSignedIn() const noexcept = 0;
    virtual AchievementsManager& achievements() noexcept = 0;
    virtual LeaderboardsManager& leaderboards() noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_ACHIEVEMENTS_SERVICE_H
