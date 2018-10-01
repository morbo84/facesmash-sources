#ifndef FACE_SMASH_SERVICE_GAME_SERVICES_H
#define FACE_SMASH_SERVICE_GAME_SERVICES_H


#include "../common/types.h"
#include <SDL_types.h>
#include <cstdint>
#include <vector>
#include <functional>


namespace gamee {


enum class FaceSmashLeaderboard {
    SCORE, FACES
};


struct AchievementsService {
    virtual ~AchievementsService() = default;
    virtual void increment(FaceSmashAchievement, uint32_t = 1) noexcept = 0;
    virtual void unlock(FaceSmashAchievement) noexcept = 0;
    virtual void showAllUI() noexcept = 0;
    virtual void query(FaceSmashAchievement, std::function<void(bool)>) = 0;
};


struct LeaderboardsService {
    virtual ~LeaderboardsService() = default;
    virtual void submitScore(FaceSmashLeaderboard, uint64_t) noexcept = 0;
    virtual void showAllLeaderboardsUI() noexcept = 0;
};


struct MultiplayerService {
    virtual ~MultiplayerService() = default;
    virtual void quickMatch() noexcept = 0;
    virtual void inviteFriend() noexcept = 0;
    virtual void invitationInbox() noexcept = 0;
    virtual void terminateMatch() noexcept = 0;
    virtual void getOpponentFrame(std::function<void(const void *, int)>) noexcept = 0;
    virtual void getOpponentScore(std::function<void(int)>) const noexcept = 0;
    virtual void sendCameraFrame(const void *, int) = 0;
    virtual void sendPlayerScore(int) = 0;
    virtual bool readyPlayerOne() const noexcept = 0;
    virtual void fetchInvitations() noexcept = 0;
    virtual void handleInvitations() noexcept = 0;
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

    virtual AchievementsService& achievements() noexcept = 0;
    virtual LeaderboardsService& leaderboards() noexcept = 0;
    virtual MultiplayerService& multiplayer() noexcept = 0;
};


}


#endif // FACE_SMASH_SERVICE_GAME_SERVICES_H
