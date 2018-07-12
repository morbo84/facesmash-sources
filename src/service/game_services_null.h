#ifndef FACE_SMASH_SERVICE_ACHIEVEMENTS_NULL_H
#define FACE_SMASH_SERVICE_ACHIEVEMENTS_NULL_H


#include "game_services_service.h"


namespace gamee {


struct GameServicesNull : GameServicesService, AchievementsManager, LeaderboardsManager {
    Status status() const noexcept override;

    void signIn() noexcept override;
    void signOut() noexcept override;
    bool isSignedIn() const noexcept override;
    AchievementsManager& achievements() noexcept override;
    LeaderboardsManager& leaderboards() noexcept override;

    void query(FaceSmashAchievement) noexcept override;
    void increment(FaceSmashAchievement a, uint32_t steps) noexcept override;
    void unlock(FaceSmashAchievement a) noexcept override;
    void showAllUI() noexcept override;

    void submitScore(FaceSmashLeaderboard, uint64_t) noexcept override;
    void showAllLeaderboardsUI() noexcept override;
};


}


#endif // FACE_SMASH_SERVICE_ACHIEVEMENTS_NULL_H
