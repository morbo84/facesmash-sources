#ifndef FACE_SMASH_GAME_GAME_LOOP_H
#define FACE_SMASH_GAME_GAME_LOOP_H


#include <memory>
#include "../types/types.hpp"
#include "game_env.h"


namespace gamee {


struct GameRenderer;
struct SceneEvent;
struct Scene;


class GameLoop final: public GameEnv {
    void setup() override;
    void shutdown() override;

    void update(GameRenderer &, delta_type) override;

public:
    GameLoop();
    ~GameLoop();

    void receive(const SceneEvent &) noexcept;

private:
    std::unique_ptr<Scene> current;
};


}


#endif // FACE_SMASH_GAME_GAME_LOOP_H
