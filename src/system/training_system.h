#ifndef FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
#define FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H


#include <SDL_types.h>
#include "../common/types.h"


namespace gamee {


class FaceRequest;
class FaceEvent;
class Spawner;


class TrainingSystem final {
    static constexpr delta_type expectation = 250_ui32;
    static constexpr delta_type duration = 10000_ui32;
    static constexpr delta_type bonus = 2000_ui32;
    static constexpr float factor = .009f;

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
    float target;
    float total;
    Uint16 counter;
    FaceType current;
};


}


#endif // FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
