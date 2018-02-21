#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../factory/spawner.h"
#include "../locator/locator.hpp"
#include "the_game_system.h"


namespace gamee {


TheGameSystem::TheGameSystem()
    : timeIsOver{false}
{
    Locator::Dispatcher::ref().connect<TimeIsOverEvent>(this);
}


TheGameSystem::~TheGameSystem() {
    Locator::Dispatcher::ref().disconnect<TimeIsOverEvent>(this);
}


void TheGameSystem::receive(const TimeIsOverEvent &) noexcept {
    timeIsOver = true;
}


void TheGameSystem::update(Registry &registry, Spawner &spawner) {
    if(registry.has<LetsPlay>()) {
        if(timeIsOver) {
            Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::GAME_OVER);
        } else {
            const auto &timer = registry.get<Timer>();

            if(timer.remaining > 30000 && !registry.size<Face>()) {
                spawner.spawnFaceBottom(registry, 50_ui8, 10_ui8, faceBag.get());
            } else if(timer.remaining <= 30000 && registry.size<Face>() < 2) {
                spawner.spawnFaceBottom(registry, 100_ui8, 20_ui8, faceBag.get());
            }

            const auto &score = registry.get<PlayerScore>();

            if(timer.remaining > 50000 && timer.remaining < 55000) {
                if(registry.size<Face>() < 2
                        && !registry.size<Item>()
                        && score.hitHappy > score.hitAngry
                        && score.hitHappy > score.hitDisgusted
                        && score.hitHappy > score.hitSurprised
                        && score.hitHappy > score.hitFearful
                        && score.hitHappy > score.hitSad)
                {
                    spawner.spawnItemBottom(registry, itemBag.get());
                }

                if(registry.size<Face>() < 2 && score.hitAngry == score.hitHappy) {
                    spawner.spawnFaceLeft(registry, 50_ui8, 10_ui8, faceBag.get());
                    spawner.spawnFaceRight(registry, 50_ui8, 10_ui8, faceBag.get());
                }
            }

            if(timer.remaining > 40000 && timer.remaining < 45000) {
                if(registry.size<Face>() < 3 && !registry.size<Item>()) {
                    spawner.spawnItemTop(registry, itemBag.get());
                }

                if(registry.size<Face>() < 2) {
                    if(!(score.hitFearful % 3) || !(score.hitFearful % 7)) {
                        spawner.spawnFaceTop(registry, 50_ui8, 10_ui8, faceBag.get());
                        spawner.spawnFaceBottom(registry, 50_ui8, 10_ui8, faceBag.get());
                    } else if(!(score.hitSurprised % 3)) {
                        spawner.spawnFaceBottom(registry, 50_ui8, 20_ui8, faceBag.get());
                        spawner.spawnFaceBottom(registry, 50_ui8, 20_ui8, faceBag.get());
                    }
                }
            }

            if(timer.remaining > 30000 && timer.remaining < 32500) {
                if(registry.size<Face>() < 2) {
                    if(score.hitDisgusted > score.hitHappy) {
                        spawner.spawnFaceBottom(registry, 100_ui8, 50_ui8, faceBag.get());
                        spawner.spawnFaceBottom(registry, 100_ui8, 50_ui8, faceBag.get());
                    } else if(score.hitSad > score.hitHappy) {
                        spawner.spawnFaceTop(registry, 100_ui8, 10_ui8, faceBag.get());
                    } else if(score.hitHappy > 8) {
                        spawner.spawnFaceLeft(registry, 250_ui8, 10_ui8, faceBag.get());
                        spawner.spawnFaceRight(registry, 250_ui8, 10_ui8, faceBag.get());
                        spawner.spawnItemBottom(registry, itemBag.get());
                    } else {
                        spawner.spawnFaceBottom(registry, 50_ui8, 100_ui8, faceBag.get());
                    }
                }
            }

            if(timer.remaining > 20000 && timer.remaining < 22500) {
                if(registry.size<Face>() < 3) {
                    if(score.score > 7000) {
                        spawner.spawnFaceRight(registry, 250_ui8, 100_ui8, faceBag.get());
                        spawner.spawnFaceLeft(registry, 250_ui8, 100_ui8, faceBag.get());
                    } else if(score.score > 5000) {
                        spawner.spawnFaceTop(registry, 200_ui8, 50_ui8, faceBag.get());
                    } else if(score.score > 3000) {
                        spawner.spawnFaceBottom(registry, 50_ui8, 10_ui8, faceBag.get());
                    }

                    if((score.score % 1000) < 250 && !registry.size<Item>()) {
                        spawner.spawnItemTop(registry, itemBag.get());
                    }
                }
            }

            if(timer.remaining > 10000 && timer.remaining < 15000) {
                if((timer.remaining % 1000) < 250 && registry.size<Face>() < 5) {
                    spawner.spawnFaceBottom(registry, 250_ui8, 10_ui8, faceBag.get());
                    spawner.spawnFaceLeft(registry, 250_ui8, 10_ui8, faceBag.get());
                    spawner.spawnFaceRight(registry, 250_ui8, 10_ui8, faceBag.get());
                }

                if((timer.remaining % 1000) < 50 && !registry.size<Item>()) {
                    spawner.spawnItemTop(registry, itemBag.get());
                }
            }

            if(timer.remaining < 5000) {
                if(registry.size<Face>() < 6) {
                    spawner.spawnFaceBottom(registry, 250_ui8, 100_ui8, faceBag.get());
                    spawner.spawnFaceBottom(registry, 250_ui8, 100_ui8, faceBag.get());
                    spawner.spawnFaceBottom(registry, 250_ui8, 100_ui8, faceBag.get());
                    spawner.spawnFaceBottom(registry, 250_ui8, 100_ui8, faceBag.get());
                    spawner.spawnFaceBottom(registry, 250_ui8, 100_ui8, faceBag.get());

                    if(score.score < 750 && !registry.size<Item>()) {
                        spawner.spawnItemTop(registry, itemBag.get());
                    }
                }
            }
        }
    }

    timeIsOver = false;
}


}
