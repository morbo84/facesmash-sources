#ifndef FACE_SMASH_EVENT_EVENT_HPP
#define FACE_SMASH_EVENT_EVENT_HPP


#include <SDL_rect.h>
#include "../common/types.h"


namespace gamee {


struct QuitEvent {};


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
    float probability;
    std::unique_ptr<unsigned char[]> frame;
};


struct FaceRequest {
    FaceType type;
};


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


struct BonusEvent {
    enum class Type: Uint8 {
        SMASH_ALL,
        I_AM_RICH
    };

    const Type type;
};


struct ArmageddonEvent {};


struct FrameAvailableEvent {};
struct ActivateEasterEggEvent {};
struct CameraInitEvent {};


struct TimeIsOverEvent {
    bool forced{false};
};


struct AudioEvent {
    enum class Type: Uint8 {
        START,
        STOP
    };

    const Type type;
};


struct AudioMusicEvent {
    const AudioMusicType type;
    bool playOrEntering;
};


struct AvRecorderEvent {
    enum class Type: Uint8 {
        EXPORT,
        DISABLE,
        ENABLE
    };

    const Type type;
};


struct WallpaperEvent {
    enum class Type: Uint8 {
        EXPORT
    };

    const Type type;
};


struct PermissionEvent {
    PermissionType permission;
    PermissionStatus result;
};


struct BillingEvent {
    enum class Type: Uint8 {
        PURCHASE_OK = 0,
        PURCHASE_CANCELED = 1, // canceled by the user
        PURCHASE_ERROR = 2,
        ALREADY_PURCHASED = 3,
        NOT_PURCHASED = 4
    };

    const Product product;
    const Type type;
};


}


#endif // FACE_SMASH_EVENT_EVENT_HPP
