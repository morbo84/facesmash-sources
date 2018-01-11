#include <cassert>
#include "../component/component.hpp"
#include "../factory/factory.h"
#include "the_game_system.h"


namespace gamee {


void TheGameSystem::update(Registry &registry, Factory &factory) {
    if(registry.has<LetsPlay>()) {
        const auto &remaining = registry.get<GameTimer>().remaining;
        auto &play = registry.get<LetsPlay>();

        if(remaining > 30000 && registry.size<Face>() < 1) {
            factory.spawnFaceBottom(registry, 50_ui8, 10_ui8);
        } else if(remaining <= 30000 && registry.size<Face>() < 2) {
            factory.spawnFaceBottom(registry, 100_ui8, 20_ui8);
        }

        if(remaining < 55000 && !play.remaining55000) {
            factory.spawnFaceLeft(registry, 50_ui8, 10_ui8);
            factory.spawnFaceLeft(registry, 50_ui8, 10_ui8);
            factory.spawnFaceRight(registry, 50_ui8, 10_ui8);
            factory.spawnFaceRight(registry, 50_ui8, 10_ui8);
            factory.spawnItemTop(registry);
            play.remaining55000 = true;
        }

        if(remaining < 50000 && !play.remaining50000) {
            factory.spawnFaceBottom(registry, 200_ui8, 50_ui8);
            factory.spawnFaceBottom(registry, 200_ui8, 50_ui8);
            factory.spawnItemTop(registry);
            factory.spawnItemTop(registry);
            play.remaining50000 = true;
        }

        if(remaining < 40000 && !play.remaining40000) {
            factory.spawnFaceLeft(registry, 100_ui8, 20_ui8);
            factory.spawnFaceRight(registry, 100_ui8, 20_ui8);
            factory.spawnFaceTop(registry, 200_ui8, 50_ui8);
            factory.spawnItemBottom(registry);
            factory.spawnItemBottom(registry);
            play.remaining40000 = true;
        }

        if(remaining < 30000 && !play.remaining30000) {
            factory.spawnItemTop(registry);
            factory.spawnItemTop(registry);
            play.remaining30000 = true;
        }

        if(remaining < 15000 && !play.remaining15000) {
            factory.spawnFaceLeft(registry, 100_ui8, 20_ui8);
            factory.spawnFaceRight(registry, 100_ui8, 20_ui8);
            factory.spawnFaceTop(registry, 200_ui8, 50_ui8);
            factory.spawnItemBottom(registry);
            play.remaining15000 = true;
        }

        if(remaining < 7500) {
            if(!play.remaining7500) {
                factory.spawnItemBottom(registry);
                play.remaining7500 = true;
            }

            if(registry.size<Face>() < 3) {
                factory.spawnFaceBottom(registry, 250_ui8, 100_ui8);
            }
        }

        const auto &score = registry.get<PlayerScore>();

        if(score.score > play.nextScoreStep) {
            if(play.nextScoreStep > 5000) {
                factory.spawnFaceLeft(registry, 200_ui8, 100_ui8);
                factory.spawnFaceRight(registry, 200_ui8, 100_ui8);
            } else {
                factory.spawnFaceLeft(registry, 100_ui8, 100_ui8);
                factory.spawnFaceRight(registry, 100_ui8, 100_ui8);
            }

            play.nextScoreStep += 2500;
        }
    }
}


}
