#include "../locator/locator.hpp"
#include "achievements_system.h"


namespace gamee {


AchievementsSystem::AchievementsSystem() noexcept
    : dirtySmashEvent{false},
      dirtyGameOver{false}
{
    Locator::Dispatcher::ref().connect<SmashEvent>(this);
}


AchievementsSystem::~AchievementsSystem() noexcept {
    Locator::Dispatcher::ref().disconnect<SmashEvent>(this);
}


void AchievementsSystem::receive(const SceneChangeEvent &event) noexcept {
    current = event.scene;

    if(current == SceneType::GAME_OVER) {
        dirtyGameOver = true;
        gameOverEvent = event;
    }
}


void AchievementsSystem::receive(const SmashEvent &event) noexcept {
    dirtySmashEvent = true;
    smashEvent = event;
}


void AchievementsSystem::faceSmashSupporter(Registry &registry) {
    // TODO
}


void AchievementsSystem::myFirstCombo(Registry &registry) {
    // TODO
}


void AchievementsSystem::collector2x(Registry &registry) {
    // TODO
}


void AchievementsSystem::collector3x(Registry &registry) {
    // TODO
}


void AchievementsSystem::collector4x(Registry &registry) {
    // TODO
}


void AchievementsSystem::collector5x(Registry &registry) {
    // TODO
}


void AchievementsSystem::smashMeBaby(Registry &registry) {
    // TODO
}


void AchievementsSystem::theSniper(Registry &registry) {
    // TODO
}


void AchievementsSystem::godSmasher(Registry &registry) {
    // TODO
}


void AchievementsSystem::angrySmash(Registry &registry) {
    // TODO
}


void AchievementsSystem::disgustedSmash(Registry &registry) {
    // TODO
}


void AchievementsSystem::fearfulSmash(Registry &registry) {
    // TODO
}


void AchievementsSystem::happySmash(Registry &registry) {
    // TODO
}


void AchievementsSystem::sadSmash(Registry &registry) {
    // TODO
}


void AchievementsSystem::surprisedSmash(Registry &registry) {
    // TODO
}


void AchievementsSystem::midnightCombo(Registry &registry) {
    // TODO
}


void AchievementsSystem::noonCombo(Registry &registry) {
    // TODO
}


void AchievementsSystem::iAmSoHappy(Registry &registry) {
    // TODO
}


void AchievementsSystem::blueIsTheNewSmash(Registry &registry) {
}


void AchievementsSystem::noPainNoGame(Registry &registry) {
    // TODO
}


void AchievementsSystem::allInARow(Registry &registry) {
    // TODO
}


void AchievementsSystem::kindergarten(Registry &registry) {
    // TODO
}


void AchievementsSystem::readyToSmash(Registry &registry) {
    // TODO
}


void AchievementsSystem::smashIsMyJob(Registry &registry) {
    // TODO
}


void AchievementsSystem::update(Registry &registry) {
    faceSmashSupporter(registry);

    if(dirtySmashEvent) {
        if(current == SceneType::THE_GAME) {
            myFirstCombo(registry);
            collector2x(registry);
            collector3x(registry);
            collector4x(registry);
            collector5x(registry);
            smashMeBaby(registry);
            theSniper(registry);
            godSmasher(registry);
            angrySmash(registry);
            disgustedSmash(registry);
            fearfulSmash(registry);
            happySmash(registry);
            sadSmash(registry);
            surprisedSmash(registry);
            midnightCombo(registry);
            noonCombo(registry);
            kindergarten(registry);
            readyToSmash(registry);
            smashIsMyJob(registry);
            allInARow(registry);
        } else if(current == SceneType::TRAINING) {
            noPainNoGame(registry);
        }
    }

    if(dirtyGameOver) {
        iAmSoHappy(registry);
        blueIsTheNewSmash(registry);
    }

    dirtySmashEvent = false;
    dirtyGameOver = false;
}


}
