#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "achievements_system.h"


namespace gamee {


int total(const PlayerScore &playerScore) {
    return playerScore.hitAngry
            + playerScore.hitDisgusted
            + playerScore.hitFearful
            + playerScore.hitHappy
            + playerScore.hitSad
            + playerScore.hitSurprised;
}


// thanks to our supporters
void faceSmashSupporter(Registry &registry) {
    // TODO WTF still to do the dedicated page... coming soon, forget it in the meantime :-)
}


// easy to achieve, it gratifies the user immediately
void myFirstCombo(Registry &registry) {
    auto &playerScore = registry.get<PlayerScore>();

    if(playerScore.combo2x
            || playerScore.combo3x
            || playerScore.combo4x
            || playerScore.combo5x)
    {
        // TODO got the achievement My First Combo
    }
}


// collector, 50 combo of the given type
void collector2x(Registry &registry) {
    if(registry.get<PlayerScore>().combo2x) {
        // TODO got part of the achievement 2x Combo Collector
    }
}


// collector, 40 combo of the given type
void collector3x(Registry &registry) {
    if(registry.get<PlayerScore>().combo2x) {
        // TODO got part of the achievement 3x Combo Collector
    }
}


// collector, 30 combo of the given type
void collector4x(Registry &registry) {
    if(registry.get<PlayerScore>().combo2x) {
        // TODO got part of the achievement 4x Combo Collector
    }
}


// collector, 20 combo of the given type
void collector5x(Registry &registry) {
    if(registry.get<PlayerScore>().combo2x) {
        // TODO got part of the achievement 5x Combo Collector
    }
}


// my first smash
void smashMeBaby(Registry &registry) {
    auto &playerScore = registry.get<PlayerScore>();

    if(playerScore.hitHappy
            || playerScore.hitAngry
            || playerScore.hitDisgusted
            || playerScore.hitSurprised
            || playerScore.hitFearful
            || playerScore.hitSad)
    {
        // TODO got the achievement Smash Me, Baby
    }
}


// 100th smash
void littleSmasher(Registry &registry) {
    const auto smash = total(registry.get<PlayerScore>());

    if(smash) {
        // TODO got part of the achievement Little Smasher
    }
}


// 1000th smash
void theSniper(Registry &registry) {
    const auto smash = total(registry.get<PlayerScore>());

    if(smash) {
        // TODO got part of the achievement The Sniper
    }
}


// 10000th smash
void godSmasher(Registry &registry) {
    const auto smash = total(registry.get<PlayerScore>());

    if(smash) {
        // TODO got part of the achievement God Smasher
    }
}


// 100 smashes of the given type
void angrySmash(Registry &registry) {
    const auto smash = registry.get<PlayerScore>().hitAngry;

    if(smash) {
        // TODO got part of the achievement Smash the Angry
    }
}


// 100 smashes of the given type
void disgustedSmash(Registry &registry) {
    const auto smash = registry.get<PlayerScore>().hitDisgusted;

    if(smash) {
        // TODO got part of the achievement Smash the Disgusted
    }
}


// 100 smashes of the given type
void fearfulSmash(Registry &registry) {
    const auto smash = registry.get<PlayerScore>().hitFearful;

    if(smash) {
        // TODO got part of the achievement Smash the Fearful
    }
}


// 100 smashes of the given type
void happySmash(Registry &registry) {
    const auto smash = registry.get<PlayerScore>().hitHappy;

    if(smash) {
        // TODO got part of the achievement Smash the Happy
    }
}


// 100 smashes of the given type
void sadSmash(Registry &registry) {
    const auto smash = registry.get<PlayerScore>().hitSad;

    if(smash) {
        // TODO got part of the achievement Smash the Sad
    }
}


// 100 smashes of the given type
void surprisedSmash(Registry &registry) {
    const auto smash = registry.get<PlayerScore>().hitSurprised;

    if(smash) {
        // TODO got part of the achievement Smash the Surprised
    }
}


// smash only happy faces in a match
void iAmSoHappy(Registry &registry) {
    auto &playerScore = registry.get<PlayerScore>();

    if(playerScore.hitHappy == total(playerScore)) {
        // TODO got the achievement I'm so Happy
    }
}


// smash only sad faces in a match
void blueIsTheNewSmash(Registry &registry) {
    auto &playerScore = registry.get<PlayerScore>();

    if(playerScore.hitSad == total(playerScore)) {
        // TODO got the achievement Blue is the new Smash
    }
}


// smash a face during the training
void noPainNoGame(Registry &registry) {
    auto &playerScore = registry.get<PlayerScore>();

    if(playerScore.hitSad
            || playerScore.hitAngry
            || playerScore.hitDisgusted
            || playerScore.hitSurprised
            || playerScore.hitFearful
            || playerScore.hitHappy)
    {
        // TODO got the achievement No Pain, No Game
    }
}


// my first 1000 points
void kindergarten(Registry &registry) {
    if(registry.get<PlayerScore>().score > 1000) {
        // TODO got the achievement Kindergarten
    }
}


// my first 10000 points
void readyToSmash(Registry &registry) {
    if(registry.get<PlayerScore>().score > 10000) {
        // TODO got the achievement Ready to Smash
    }
}


// my first 25000 points
void smashIsMyJob(Registry &registry) {
    if(registry.get<PlayerScore>().score > 25000) {
        // TODO got the achievement Smash is my Job
    }
}


// 50 smashes or more in a match
void ohMySmash(Registry &registry) {
    if(50 <= total(registry.get<PlayerScore>())) {
        // TODO got the achievement Oh My Smash
    }
}


// no smashes in a match
void smashMeCry(Registry &registry) {
    if(0 == total(registry.get<PlayerScore>())) {
        // TODO got the achievement Smash Me Cry
    }
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


void AchievementsSystem::update(Registry &registry) {
    faceSmashSupporter(registry);

    if(registry.has<PlayerScore>()) {
        if(current == SceneType::THE_GAME) {
            myFirstCombo(registry);
            collector2x(registry);
            collector3x(registry);
            collector4x(registry);
            collector5x(registry);
            smashMeBaby(registry);
            littleSmasher(registry);
            theSniper(registry);
            godSmasher(registry);
            angrySmash(registry);
            disgustedSmash(registry);
            fearfulSmash(registry);
            happySmash(registry);
            sadSmash(registry);
            surprisedSmash(registry);
            kindergarten(registry);
            readyToSmash(registry);
            smashIsMyJob(registry);
        } else if(current == SceneType::TRAINING) {
            noPainNoGame(registry);
        }

        if(dirtyGameOver) {
            ohMySmash(registry);
            iAmSoHappy(registry);
            blueIsTheNewSmash(registry);
            smashMeCry(registry);
        }
    }

    dirtyGameOver = false;
}


}
