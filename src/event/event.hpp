#ifndef FACE_SMASH_EVENT_EVENT_HPP
#define FACE_SMASH_EVENT_EVENT_HPP


#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct EnvEvent {
    enum class Type: Uint8 {
        TERMINATING,
        LOW_MEMORY,
        ENTERING_BACKGROUND,
        ENTERED_BACKGROUND,
        ENTERING_FOREGROUND,
        ENTERED_FOREGROUND
    };

    const Type type;
};


struct KeyboardEvent {
    enum class Type: Uint8 {
        ESCAPE,
        BACK
    };

    const Type type;
};


struct TouchEvent final: SDL_Point {};


struct SceneChangeEvent {
    SceneType scene;
};


struct FaceEvent {
    FaceType type;
};


// different meaning, same structure... why not? :-)
struct FaceRequest: FaceEvent {};


struct SmashEvent {
    int angry;
    int disgusted;
    int happy;
    int surprised;
    int fearful;
    int sad;
    int smash;
    int miss;
    int combo;
};


struct FrameAvailableEvent {};
struct ActivateEasterEggEvent {};
struct TimeIsOverEvent {};


struct GameServicesEvent {
    enum class Type: Uint8 {
        SIGNING_IN,
        SIGNED_IN,
        SIGNING_OUT,
        SIGNED_OUT
    };

    const Type type;
};


struct AudioEvent {
    enum class Type: Uint8 {
        START,
        STOP
    };

    const Type type;
};


struct AvRecorderEvent {
    enum class Type: Uint8 {
        EXPORT,
        DISABLE,
        ENABLE
    };

    const Type type;
};


struct PermissionEvent {
    PermissionType permission;
    PermissionStatus result;
};


}


#endif // FACE_SMASH_EVENT_EVENT_HPP
