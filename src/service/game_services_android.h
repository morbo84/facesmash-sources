#ifndef FACE_SMASH_SERVICE_ACHIEVEMENTS_ANDROID_H
#define FACE_SMASH_SERVICE_ACHIEVEMENTS_ANDROID_H


#include "game_services_service.h"
#include <memory>

#ifdef __ANDROID__
#include <mutex>
#endif


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
    Status status() const noexcept override;
    void signIn() noexcept override;
    void signOut() noexcept override;
    bool isSignedIn() const noexcept override;

    AchievementsManager& achievements() noexcept override;
    LeaderboardsManager& leaderboards() noexcept override;

    // AchievementsManager interface
    void increment(FaceSmashAchievement, uint32_t) noexcept override;
    void unlock(FaceSmashAchievement) noexcept override;
    void showAllUI() noexcept override;
    void query(FaceSmashAchievement, std::function<void(bool)>) override;

    // LeaderboardsManager interface
    void submitScore(FaceSmashLeaderboard, uint64_t) noexcept override;
    void showAllLeaderboardsUI() noexcept override;

private:
    bool isAuthorized() const noexcept;

    std::unique_ptr<gpg::GameServices> gs_;
    Status status_;
    bool ready_;
#ifdef __ANDROID__
    mutable std::mutex mutex;
#endif
};


}


#endif // FACE_SMASH_SERVICE_ACHIEVEMENTS_ANDROID_H
