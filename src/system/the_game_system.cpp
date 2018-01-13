#include "../component/component.hpp"
#include "../factory/factory.h"
#include "the_game_system.h"


namespace gamee {


void TheGameSystem::update(Registry &registry, Factory &factory) {
    if(registry.has<LetsPlay>()) {
        const auto &remaining = registry.get<GameTimer>().remaining;
        auto &play = registry.get<LetsPlay>();

        if(remaining > 30000 && registry.size<Face>() < 1) {
            factory.spawnFaceBottom(registry, 50_ui8, 10_ui8, faceBag.get());
        } else if(remaining <= 30000 && registry.size<Face>() < 2) {
            factory.spawnFaceBottom(registry, 100_ui8, 20_ui8, faceBag.get());
        }

        if(remaining < 55000 && !play.remaining55000) {
            factory.spawnFaceLeft(registry, 50_ui8, 10_ui8, faceBag.get());
            factory.spawnFaceLeft(registry, 50_ui8, 10_ui8, faceBag.get());
            factory.spawnFaceRight(registry, 50_ui8, 10_ui8, faceBag.get());
            factory.spawnFaceRight(registry, 50_ui8, 10_ui8, faceBag.get());
            factory.spawnItemTop(registry, itemBag.get());
            play.remaining55000 = true;
        }

        if(remaining < 50000 && !play.remaining50000) {
            factory.spawnFaceBottom(registry, 200_ui8, 50_ui8, faceBag.get());
            factory.spawnFaceBottom(registry, 200_ui8, 50_ui8, faceBag.get());
            factory.spawnItemTop(registry, itemBag.get());
            factory.spawnItemTop(registry, itemBag.get());
            play.remaining50000 = true;
        }

        if(remaining < 40000 && !play.remaining40000) {
            factory.spawnFaceLeft(registry, 100_ui8, 20_ui8, faceBag.get());
            factory.spawnFaceRight(registry, 100_ui8, 20_ui8, faceBag.get());
            factory.spawnFaceTop(registry, 200_ui8, 50_ui8, faceBag.get());
            factory.spawnItemBottom(registry, itemBag.get());
            factory.spawnItemBottom(registry, itemBag.get());
            play.remaining40000 = true;
        }

        if(remaining < 30000 && !play.remaining30000) {
            factory.spawnItemTop(registry, itemBag.get());
            factory.spawnItemTop(registry, itemBag.get());
            play.remaining30000 = true;
        }

        if(remaining < 15000 && !play.remaining15000) {
            factory.spawnFaceLeft(registry, 100_ui8, 20_ui8, faceBag.get());
            factory.spawnFaceRight(registry, 100_ui8, 20_ui8, faceBag.get());
            factory.spawnFaceTop(registry, 200_ui8, 50_ui8, faceBag.get());
            factory.spawnItemBottom(registry, itemBag.get());
            play.remaining15000 = true;
        }

        if(remaining < 7500) {
            if(!play.remaining7500) {
                factory.spawnItemBottom(registry, itemBag.get());
                play.remaining7500 = true;
            }

            if(registry.size<Face>() < 3) {
                factory.spawnFaceBottom(registry, 250_ui8, 100_ui8, faceBag.get());
            }
        }

        const auto &score = registry.get<PlayerScore>();

        if(score.score > play.nextScoreStep) {
            if(play.nextScoreStep > 5000) {
                factory.spawnFaceLeft(registry, 200_ui8, 100_ui8, faceBag.get());
                factory.spawnFaceRight(registry, 200_ui8, 100_ui8, faceBag.get());
            } else {
                factory.spawnFaceLeft(registry, 100_ui8, 100_ui8, faceBag.get());
                factory.spawnFaceRight(registry, 100_ui8, 100_ui8, faceBag.get());
            }

            play.nextScoreStep += 2500;
        }
    }
}


}
