#ifndef FACE_SMASH_SERVICE_ACHIEVEMENTS_SERVICE_H
#define FACE_SMASH_SERVICE_ACHIEVEMENTS_SERVICE_H


#include "../common/types.h"
#include <SDL_types.h>
#include <cstdint>


namespace gamee {


enum class FaceSmashLeaderboard {
    SCORE, FACES
};


struct AchievementsManager {
    virtual ~AchievementsManager() = default;

    virtual void query(FaceSmashAchievement) noexcept = 0;
    virtual void increment(FaceSmashAchievement, uint32_t = 1) noexcept = 0;
    virtual void unlock(FaceSmashAchievement) noexcept = 0;
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
