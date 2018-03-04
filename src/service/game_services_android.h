#ifndef FACE_SMASH_SERVICE_ACHIEVEMENTS_ANDROID_H
#define FACE_SMASH_SERVICE_ACHIEVEMENTS_ANDROID_H


#include "game_services_service.h"
#include <memory>


namespace gpg {
class GameServices;
}


namespace gamee {


struct GameServicesAndroid : GameServicesService, AchievementsManager, LeaderboardsManager {
    GameServicesAndroid() noexcept;
    GameServicesAndroid& operator=(const GameServicesAndroid&) = default;
    GameServicesAndroid& operator=(GameServicesAndroid&&) = default;
    ~GameServicesAndroid() noexcept override;

    // GameServicesService interface
    void signIn() noexcept override;
    void signOut() noexcept override;
    bool isSignedIn() const noexcept override;
    bool isAvailable() const noexcept override;
    AchievementsManager& achievements() noexcept override;
    LeaderboardsManager& leaderboards() noexcept override;

    // AchievementsManager interface
    void increment(FaceSmashAchievement a, uint32_t steps) noexcept override;
    void unlock(FaceSmashAchievement a) noexcept override;
    void showAllUI() noexcept override;

    // LeaderboardsManager interface
    void submitScore(FaceSmashLeaderboard, uint64_t) noexcept override;
    void showAllLeaderboardsUI() noexcept override;

private:
    bool isAuthorized() const noexcept;

    std::unique_ptr<gpg::GameServices> gs_;
    bool ready_;
};


}


#endif // FACE_SMASH_SERVICE_ACHIEVEMENTS_ANDROID_H