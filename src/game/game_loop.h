#ifndef FACE_SMASH_GAME_GAME_LOOP_H
#define FACE_SMASH_GAME_GAME_LOOP_H


#include <memory>
#include "../common/types.h"
#include "game_env.h"


namespace gamee {


struct GameRenderer;
struct SceneEvent;
struct Scene;


class GameLoop final: public GameEnv {
    void update(GameRenderer &, delta_type) override;

public:
    GameLoop();
    ~GameLoop();

    void receive(const SceneEvent &) noexcept;

private:
    std::unique_ptr<Scene> current;
    std::unique_ptr<Scene> next;
    bool dirty;
};


}


#endif // FACE_SMASH_GAME_GAME_LOOP_H
