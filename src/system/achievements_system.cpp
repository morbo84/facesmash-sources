#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "../service/game_services_service.h"
#include "achievements_system.h"


namespace gamee {


static PlayerScore operator-(const PlayerScore& s1, const PlayerScore& s2) {
    return {
            s1.score - s2.score,
            s1.hitAngry - s2.hitAngry,
            s1.hitDisgusted - s2.hitDisgusted,
            s1.hitHappy - s2.hitHappy,
            s1.hitSurprised - s2.hitSurprised,
            s1.hitFearful - s2.hitFearful,
            s1.hitSad - s2.hitSad,
            s1.combo2x - s2.combo2x,
            s1.combo3x - s2.combo3x,
            s1.combo4x - s2.combo4x,
            s1.combo5x - s2.combo5x
    };
}


int total(const PlayerScore &playerScore) {
    return playerScore.hitAngry
            + playerScore.hitDisgusted
            + playerScore.hitFearful
            + playerScore.hitHappy
            + playerScore.hitSad
            + playerScore.hitSurprised;
}


void faceSmashSupporter() {
    Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::FACE_SMASH_SUPPORTER);
}


// easy to achieve, it gratifies the user immediately
void myFirstCombo(const PlayerScore& score) {
    if(score.combo2x || score.combo3x || score.combo4x || score.combo5x) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::MY_FIRST_COMBO);
    }
}


// my first smash
void smashMeBaby(const PlayerScore& score) {
    if(score.hitHappy
       || score.hitAngry
       || score.hitDisgusted
       || score.hitSurprised
       || score.hitFearful
       || score.hitSad)
    {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::SMASH_ME_BABY);
    }
}


// my first 1000 points
void kindergarten(const PlayerScore& score) {
    if(score.score >= 1000) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::KINDERGARTEN);
    }
}


// my first 10000 points
void readyToSmash(const PlayerScore& score) {
    if(score.score >= 10000) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::READY_TO_SMASH);
    }
}


// my first 25000 points
void smashIsMyJob(const PlayerScore& score) {
    if(score.score >= 25000) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::SMASH_IS_MY_JOB);
    }
}


// collector, 50 combo of the given type
void collector2x(const PlayerScore& delta) {
    if(delta.combo2x) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::COMBO_COLLECTOR_2X, delta.combo2x);
    }
}


// collector, 40 combo of the given type
void collector3x(const PlayerScore& delta) {
    if(delta.combo3x) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::COMBO_COLLECTOR_3X, delta.combo3x);
    }
}


// collector, 30 combo of the given type
void collector4x(const PlayerScore& delta) {
    if(delta.combo4x) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::COMBO_COLLECTOR_4X, delta.combo4x);
    }
}


// collector, 20 combo of the given type
void collector5x(const PlayerScore& delta) {
    if(delta.combo5x) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::COMBO_COLLECTOR_5X, delta.combo5x);
    }
}


// 100th smash
void littleSmasher(const PlayerScore& delta) {
    const auto smash = total(delta);

    if(smash) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::LITTLE_SMASHER, smash);
    }
}


// 1000th smash
void theSniper(const PlayerScore& delta) {
    const auto smash = total(delta);

    if(smash) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::THE_SNIPER, smash);
    }
}


// 10000th smash
void godSmasher(const PlayerScore& delta) {
    const auto smash = total(delta);

    if(smash) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::GOD_SMASHER, smash);
    }
}


// 100 smashes of the given type
void angrySmash(const PlayerScore& delta) {
    if(delta.hitAngry) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::SMASH_THE_ANGRY, delta.hitAngry);
    }
}


// 100 smashes of the given type
void disgustedSmash(const PlayerScore& delta) {
    if(delta.hitDisgusted) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::SMASH_THE_DISGUSTED, delta.hitDisgusted);
    }
}


// 100 smashes of the given type
void fearfulSmash(const PlayerScore& delta) {
    if(delta.hitFearful) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::SMASH_THE_FEARFUL, delta.hitFearful);
    }
}


// 100 smashes of the given type
void happySmash(const PlayerScore& delta) {
    if(delta.hitHappy) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::SMASH_THE_HAPPY, delta.hitHappy);
    }
}


// 100 smashes of the given type
void sadSmash(const PlayerScore& delta) {
    if(delta.hitSad) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::SMASH_THE_SAD, delta.hitSad);
    }
}


// 100 smashes of the given type
void surprisedSmash(const PlayerScore& delta) {
    if(delta.hitSurprised) {
        Locator::GameServices::ref().achievements().increment(FaceSmashAchievement::SMASH_THE_SURPRISED, delta.hitSurprised);
    }
}


// smash a face during the training
void noPainNoGame(const PlayerScore &score) {
    if(score.hitSad
       || score.hitAngry
       || score.hitDisgusted
       || score.hitSurprised
       || score.hitFearful
       || score.hitHappy)
    {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::NO_PAIN_NO_GAME);
    }
}


// 50 smashes or more in a match
void ohMySmash(const PlayerScore &score) {
    if(50 <= total(score)) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::OH_MY_SMASH);
    }
}


// smash only happy faces in a match
void iAmSoHappy(const PlayerScore &score) {
    if(score.hitHappy && score.hitHappy == total(score)) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::IM_SO_HAPPY);
    }
}


// smash only sad faces in a match
void blueIsTheNewSmash(const PlayerScore &score) {
    if(score.hitSad && score.hitSad == total(score)) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::BLUE_SMASH);
    }
}


// no smashes in a match
void smashMeCry(const PlayerScore &score) {
    if(0 == total(score)) {
        Locator::GameServices::ref().achievements().unlock(FaceSmashAchievement::SMASH_ME_CRY);
    }
}


// submit scores to leaderboards
void submitToLeaderboards(const PlayerScore &score) {
    Locator::GameServices::ref().leaderboards().submitScore(FaceSmashLeaderboard::SCORE, score.score);
    Locator::GameServices::ref().leaderboards().submitScore(FaceSmashLeaderboard::FACES, total(score));
}


AchievementsSystem::AchievementsSystem() noexcept
    : dirtyGameOver{false}
{
    Locator::Dispatcher::ref().connect<SceneChangeEvent>(this);
}


AchievementsSystem::~AchievementsSystem() noexcept {
    Locator::Dispatcher::ref().disconnect<SceneChangeEvent>(this);
}


void AchievementsSystem::receive(const SceneChangeEvent &event) noexcept {
    current = event.scene;
    dirtyGameOver = (current == SceneType::GAME_OVER);
}


void AchievementsSystem::receive(const BillingEvent &event) noexcept {
    if(event.product == Product::REMOVE_ADS && (event.type == BillingEvent::Type::PURCHASE_OK || event.type == BillingEvent::Type::ALREADY_PURCHASED)) {
        faceSmashSupporter();
    }
}


void AchievementsSystem::update(Registry &registry) {
    if(registry.has<PlayerScore>()) {
        const auto &score = registry.get<PlayerScore>();
        auto delta = score - previous;

        if(current == SceneType::THE_GAME) {
            myFirstCombo(score);
            smashMeBaby(score);
            kindergarten(score);
            readyToSmash(score);
            smashIsMyJob(score);
            collector2x(delta);
            collector3x(delta);
            collector4x(delta);
            collector5x(delta);
            littleSmasher(delta);
            theSniper(delta);
            godSmasher(delta);
            angrySmash(delta);
            disgustedSmash(delta);
            fearfulSmash(delta);
            happySmash(delta);
            sadSmash(delta);
            surprisedSmash(delta);
            previous = score;
        } else if(current == SceneType::TRAINING) {
            noPainNoGame(score);
        }

        if(dirtyGameOver) {
            ohMySmash(score);
            iAmSoHappy(score);
            blueIsTheNewSmash(score);
            smashMeCry(score);
            submitToLeaderboards(score);
        }
    }

    dirtyGameOver = false;
}


}
