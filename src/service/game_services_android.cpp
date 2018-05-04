#include "game_services_android.h"
#include "../locator/locator.hpp"
#include "../event/event.hpp"
#include <chrono>
#ifdef __ANDROID__
#include <gpg/achievement_manager.h>
#include <gpg/builder.h>
#include <gpg/game_services.h>
#include <gpg/leaderboard_manager.h>
#include <gpg/platform_configuration.h>
#include <SDL_system.h>


namespace gamee {


static constexpr const char* achievementCode(FaceSmashAchievement a) {
    switch(a) {
    case FaceSmashAchievement::SMASH_ME_BABY:
        return "CgkI5IbdrqUREAIQAA";
    case FaceSmashAchievement::LITTLE_SMASHER:
        return "CgkI5IbdrqUREAIQAg";
    case FaceSmashAchievement::THE_SNIPER:
        return "CgkI5IbdrqUREAIQAw";
    case FaceSmashAchievement::GOD_SMASHER:
        return "CgkI5IbdrqUREAIQBA";
    case FaceSmashAchievement::MY_FIRST_COMBO:
        return "CgkI5IbdrqUREAIQBQ";
    case FaceSmashAchievement::COMBO_COLLECTOR_2X:
        return "CgkI5IbdrqUREAIQBg";
    case FaceSmashAchievement::COMBO_COLLECTOR_3X:
        return "CgkI5IbdrqUREAIQBw";
    case FaceSmashAchievement::COMBO_COLLECTOR_4X:
        return "CgkI5IbdrqUREAIQCA";
    case FaceSmashAchievement::COMBO_COLLECTOR_5X:
        return "CgkI5IbdrqUREAIQCQ";
    case FaceSmashAchievement::SMASH_THE_ANGRY:
        return "CgkI5IbdrqUREAIQCg";
    case FaceSmashAchievement::SMASH_THE_DISGUSTED:
        return "CgkI5IbdrqUREAIQCw";
    case FaceSmashAchievement::SMASH_THE_FEARFUL:
        return "CgkI5IbdrqUREAIQDA";
    case FaceSmashAchievement::SMASH_THE_HAPPY:
        return "CgkI5IbdrqUREAIQDQ";
    case FaceSmashAchievement::SMASH_THE_SAD:
        return "CgkI5IbdrqUREAIQDg";
    case FaceSmashAchievement::SMASH_THE_SURPRISED:
        return "CgkI5IbdrqUREAIQDw";
    case FaceSmashAchievement::IM_SO_HAPPY:
        return "CgkI5IbdrqUREAIQEA";
    case FaceSmashAchievement::BLUE_SMASH:
        return "CgkI5IbdrqUREAIQEQ";
    case FaceSmashAchievement::NO_PAIN_NO_GAME:
        return "CgkI5IbdrqUREAIQEg";
    case FaceSmashAchievement::KINDERGARTEN:
        return "CgkI5IbdrqUREAIQEw";
    case FaceSmashAchievement::READY_TO_SMASH:
        return "CgkI5IbdrqUREAIQFA";
    case FaceSmashAchievement::SMASH_IS_MY_JOB:
        return "CgkI5IbdrqUREAIQFQ";
    case FaceSmashAchievement::OH_MY_SMASH:
        return "CgkI5IbdrqUREAIQFg";
    case FaceSmashAchievement::SMASH_ME_CRY:
        return "CgkI5IbdrqUREAIQFw";
    case FaceSmashAchievement::FACE_SMASH_SUPPORTER:
        return "CgkI5IbdrqUREAIQHQ"
    }
}


static constexpr const char* leaderboardCode(FaceSmashLeaderboard l) {
    switch(l) {
        case FaceSmashLeaderboard::SCORE:
            return "CgkI5IbdrqUREAIQGA";
        case FaceSmashLeaderboard::FACES:
            return "CgkI5IbdrqUREAIQGQ";
    }
}


GameServicesAndroid::GameServicesAndroid() noexcept
    : ready_{true}
{}


GameServicesAndroid::~GameServicesAndroid() noexcept {}


void GameServicesAndroid::signIn() noexcept {
    if(ready_) {
        ready_ = false;

        if (!gs_) {
            gpg::GameServices::Builder builder;

            builder.SetOnAuthActionFinished([this](gpg::AuthOperation op, gpg::AuthStatus status) {
                auto &dispatcher = Locator::Dispatcher::ref();
                ready_ = true;

                switch (op) {
                    case gpg::AuthOperation::SIGN_IN:
                        switch (status) {
                        case gpg::AuthStatus::VALID:
                            dispatcher.enqueue<GameServicesEvent>(GameServicesEvent::Type::SIGNED_IN);
                            break;
                        default:
                            dispatcher.enqueue<GameServicesEvent>(GameServicesEvent::Type::SIGNED_OUT);
                            break;
                        }
                        break;
                    case gpg::AuthOperation::SIGN_OUT:
                        dispatcher.enqueue<GameServicesEvent>(GameServicesEvent::Type::SIGNED_OUT);
                        break;
                }
            });

            JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
            jobject activity = (jobject) SDL_AndroidGetActivity();
            gpg::PlatformConfiguration conf;
            conf.SetActivity(activity);
            gs_ = builder.Create(conf);
            env->DeleteLocalRef(activity);
            Locator::Dispatcher::ref().enqueue<GameServicesEvent>(GameServicesEvent::Type::SIGNING_IN);
        } else {
            gs_->StartAuthorizationUI();
        }
    }
}


void GameServicesAndroid::signOut() noexcept {
    if(isAuthorized()) {
        ready_ = false;
        gs_->SignOut();
        Locator::Dispatcher::ref().enqueue<GameServicesEvent>(GameServicesEvent::Type::SIGNING_OUT);
    }
}


void GameServicesAndroid::increment(FaceSmashAchievement a, uint32_t steps) noexcept {
    if(isAuthorized()) {
        gs_->Achievements().Increment(achievementCode(a), steps);
    }
}


void GameServicesAndroid::unlock(FaceSmashAchievement a) noexcept {
    if(isAuthorized()) {
        gs_->Achievements().Unlock(achievementCode(a));
    }
}


void GameServicesAndroid::showAllUI() noexcept {
    if(isAuthorized()) {
        gs_->Achievements().ShowAllUIBlocking(std::chrono::seconds{3});
    }
}


bool GameServicesAndroid::isSignedIn() const noexcept {
    return gs_ && gs_->IsAuthorized();
}


bool GameServicesAndroid::isAuthorized() const noexcept {
    return ready_ && gs_ && gs_->IsAuthorized();
}


AchievementsManager& GameServicesAndroid::achievements() noexcept {
    return *this;
}


LeaderboardsManager& GameServicesAndroid::leaderboards() noexcept {
    return *this;
}


void GameServicesAndroid::submitScore(FaceSmashLeaderboard l, uint64_t score) noexcept {
    if(isAuthorized()) {
        gs_->Leaderboards().SubmitScore(leaderboardCode(l), score);
    }
}


void GameServicesAndroid::showAllLeaderboardsUI() noexcept {
    if(isAuthorized()) {
        gs_->Leaderboards().ShowAllUIBlocking(std::chrono::seconds{3});
    }
}


}


#else

namespace gpg {
class GameServices {};
}

namespace gamee {

GameServicesAndroid::GameServicesAndroid() noexcept {}
GameServicesAndroid::~GameServicesAndroid() noexcept {}
void GameServicesAndroid::signIn() noexcept {}
void GameServicesAndroid::signOut() noexcept {}
void GameServicesAndroid::unlock(FaceSmashAchievement) noexcept {}
void GameServicesAndroid::increment(FaceSmashAchievement, uint32_t) noexcept {}
void GameServicesAndroid::showAllUI() noexcept {}
bool GameServicesAndroid::isSignedIn() const noexcept { return false; }
void GameServicesAndroid::submitScore(FaceSmashLeaderboard, uint64_t) noexcept {}
void GameServicesAndroid::showAllLeaderboardsUI() noexcept {}
AchievementsManager& GameServicesAndroid::achievements() noexcept { return *this; }
LeaderboardsManager& GameServicesAndroid::leaderboards() noexcept { return *this; }

}

#endif
