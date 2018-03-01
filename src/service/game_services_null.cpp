#include "game_services_null.h"


namespace gamee {


void GameServicesNull::increment(FaceSmashAchievement, uint32_t) noexcept {}


void GameServicesNull::unlock(FaceSmashAchievement) noexcept {}


void GameServicesNull::showAllUI() noexcept {}


void GameServicesNull::submitScore(FaceSmashLeaderboard, uint64_t) noexcept {}


void GameServicesNull::signIn() noexcept {}


void GameServicesNull::signOut() noexcept {}


bool GameServicesNull::isSignedIn() const noexcept { return false; }


bool GameServicesNull::isAvailable() const noexcept { return false; }


AchievementsManager& GameServicesNull::achievements() noexcept { return *this; }


LeaderboardsManager& GameServicesNull::leaderboards() noexcept { return *this; }


}
