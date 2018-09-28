#include "game_services_null.h"


namespace gamee {


void GameServicesNull::increment(FaceSmashAchievement, uint32_t) noexcept {}
void GameServicesNull::unlock(FaceSmashAchievement) noexcept {}
void GameServicesNull::showAllUI() noexcept {}
void GameServicesNull::query(FaceSmashAchievement, std::function<void(bool)> func) { func(true); }
void GameServicesNull::submitScore(FaceSmashLeaderboard, uint64_t) noexcept {}
void GameServicesNull::showAllLeaderboardsUI() noexcept {}
void GameServicesNull::quickMatch() noexcept { triggerMultiplayer = true; }
void GameServicesNull::inviteFriend() noexcept {}
void GameServicesNull::invitationInbox() noexcept {}
void GameServicesNull::terminateMatch() noexcept {}
void GameServicesNull::getOpponentFrame(std::function<void(const void *, int)> func) noexcept { func(nullptr, 0); }
void GameServicesNull::getOpponentScore(std::function<void(int)> func) const noexcept { func(100); }
void GameServicesNull::sendCameraFrame(const void *, int) {}
void GameServicesNull::sendPlayerScore(int) {}

bool GameServicesNull::readyPlayerOne() const noexcept {
    return triggerMultiplayer;
}

GameServicesService::Status GameServicesNull::status() const noexcept {  return GameServicesService::Status::SIGNED_OUT; }
void GameServicesNull::signIn() noexcept {}
void GameServicesNull::signOut() noexcept {}
bool GameServicesNull::isSignedIn() const noexcept { return true; }
AchievementsService& GameServicesNull::achievements() noexcept { return *this; }
LeaderboardsService& GameServicesNull::leaderboards() noexcept { return *this; }
MultiplayerService& GameServicesNull::multiplayer() noexcept { return *this; }


}
