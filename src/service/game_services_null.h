#ifndef FACE_SMASH_SERVICE_GAME_SERVICES_NULL_H
#define FACE_SMASH_SERVICE_GAME_SERVICES_NULL_H


#include "game_services.h"


namespace gamee {


struct GameServicesNull : GameServicesService, AchievementsService, LeaderboardsService, MultiplayerService {
    Status status() const noexcept override;

    void signIn() noexcept override;
    void signOut() noexcept override;
    bool isSignedIn() const noexcept override;

    AchievementsService& achievements() noexcept override;
    LeaderboardsService& leaderboards() noexcept override;
    MultiplayerService& multiplayer() noexcept override;

    void increment(FaceSmashAchievement a, uint32_t steps) noexcept override;
    void unlock(FaceSmashAchievement a) noexcept override;
    void showAllUI() noexcept override;
    void query(FaceSmashAchievement, std::function<void(bool)>) override;

    void submitScore(FaceSmashLeaderboard, uint64_t) noexcept override;
    void showAllLeaderboardsUI() noexcept override;

    void quickMatch() noexcept override;
    void inviteFriend() noexcept override;
    void invitationInbox() noexcept override;
    void terminateMatch() noexcept override;
    void getOpponentFrame(std::function<void(const void *, int)>) noexcept override;
    void getOpponentScore(std::function<void(int)>) const noexcept override;
    void sendCameraFrame(const void *, int) override;
    void sendPlayerScore(int) override;
    bool readyPlayerOne() const noexcept override;
    void fetchInvitations() noexcept override;
    void handleInvitations() noexcept override;

public:
    mutable bool triggerMultiplayer{false};
};


}


#endif // FACE_SMASH_SERVICE_GAME_SERVICES_NULL_H
