#ifndef FACE_SMASH_PROCESS_PROCESS_INIT_H
#define FACE_SMASH_PROCESS_PROCESS_INIT_H


#include "../common/types.h"


namespace gamee {


struct ProcessInit final: Process<ProcessInit> {
    void update(delta_type, void *);
};


}


#endif // FACE_SMASH_PROCESS_PROCESS_INIT_H
