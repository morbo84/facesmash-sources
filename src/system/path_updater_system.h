#ifndef FACE_SMASH_SYSTEM_PATH_UPDATER_SYSTEM_H
#define FACE_SMASH_SYSTEM_PATH_UPDATER_SYSTEM_H


#include "../types/types.hpp"


namespace gamee {


struct PathUpdaterSystem final {
    void update(Registry &, delta_type);
};


}


#endif // FACE_SMASH_SYSTEM_PATH_UPDATER_SYSTEM_H
