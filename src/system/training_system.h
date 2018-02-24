#ifndef FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
#define FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H


#include "../common/types.h"


namespace gamee {


class FaceRequest;
class FaceEvent;
class Spawner;


class TrainingSystem final {
    static constexpr delta_type interval = 200_ui32;
    static constexpr auto steps = 10;

    enum class TrainingState: Uint8 {
        START_TRAINING,
        TRAINING,
        STOP_TRAINING,
        IDLE
    };

    void showFaceButtons(Registry &);
    void hideFaceButtons(Registry &);

    void startTraining(Registry &);
    void training(Registry &, Spawner &, delta_type);
    void stopTraining(Registry &);
    void idle(Registry &);

public:
    TrainingSystem();
    ~TrainingSystem();

    void receive(const FaceRequest &) noexcept;
    void receive(const FaceEvent &) noexcept;

    void update(Registry &, Spawner &, delta_type);

private:
    FaceType current;
    FaceType match;
    int progress;
    delta_type remaining;
    TrainingState state;
};


}


#endif // FACE_SMASH_SYSTEM_TRAINING_SYSTEM_H
