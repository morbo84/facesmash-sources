#ifndef FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
#define FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H


#include <SDL_types.h>
#include "../common/types.h"


namespace gamee {


class FaceRequest;
class FaceEvent;
class Spawner;


class TrainingSystem final {
    static constexpr delta_type expectation = 500_ui32;
    static constexpr delta_type interval = 250_ui32;
    static constexpr Uint8 length = 32_ui32;
    static constexpr Uint8 bonus = 8_ui32;

public:
    TrainingSystem();
    ~TrainingSystem();

    void receive(const FaceRequest &) noexcept;
    void receive(const FaceEvent &) noexcept;

    void update(Registry &, Spawner &, delta_type);

private:
    delta_type watchdog;
    delta_type remaining;
    float probability;
    float total;
    Uint16 counter;
    FaceType current;
    Uint8 steps;
};


}


#endif // FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
