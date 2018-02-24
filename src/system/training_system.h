#ifndef FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
#define FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class FaceRequest;
class FaceEvent;
class Spawner;


class TrainingSystem final {
    static constexpr delta_type duration = 10000_ui32;
    static constexpr delta_type interval = 500_ui32;
    static constexpr auto steps = 10;

    enum class TrainingState: Uint8 {
        START_TRAINING,
        TRAINING,
        IDLE
    };

    void enableFaceButtons(Registry &);
    void disableFaceButtons(Registry &);

    TrainingState training(Registry &, Spawner &, delta_type);

public:
    TrainingSystem();
    ~TrainingSystem();

    void receive(const FaceRequest &) noexcept;
    void receive(const FaceEvent &) noexcept;

    void update(Registry &, Spawner &, delta_type);

private:
    FaceType current;
    FaceType match;
    Uint8 progress;
    delta_type range;
    delta_type remaining;
    TrainingState state;
};


}


#endif // FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
