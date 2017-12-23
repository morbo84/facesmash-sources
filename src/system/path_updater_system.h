#ifndef PATH_UPDATER_SYSTEM_H
#define PATH_UPDATER_SYSTEM_H


#include "../types/types.hpp"


namespace gamee {


struct PathUpdaterSystem final {
    void update(Registry &, delta_type);
};


}


#endif // PATH_UPDATER_SYSTEM_H
