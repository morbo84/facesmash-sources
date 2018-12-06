#ifndef FACE_SMASH_COMMON_TYPES_H
#define FACE_SMASH_COMMON_TYPES_H


#include <cstdint>
#include <SDL_stdinc.h>
#include <entt/entity/registry.hpp>
#include <entt/resource/cache.hpp>
#include <entt/signal/dispatcher.hpp>
#include "../resource/audio_resource.h"
#include "../resource/font_resource.h"
#include "../resource/texture_resource.h"


namespace gamee {


constexpr Uint8 operator"" _ui8 (unsigned long long ull) { return Uint8(ull); }
constexpr Sint8 operator"" _si8 (unsigned long long ull) { return Sint8(ull); }
constexpr Uint16 operator"" _ui16 (unsigned long long ull) { return Uint16(ull); }
constexpr Sint16 operator"" _si16 (unsigned long long ull) { return Sint16(ull); }
constexpr Uint32 operator"" _ui32 (unsigned long long ull) { return Uint32(ull); }
constexpr Sint32 operator"" _si32 (unsigned long long ull) { return Sint32(ull); }
constexpr Uint64 operator"" _ui64 (unsigned long long ull) { return Uint64(ull); }
constexpr Sint64 operator"" _si64 (unsigned long long ull) { return Sint64(ull); }


using delta_type = Uint32;

using AudioMusicCache = entt::resource_cache<AudioMusicResource>;
using AudioChunkCache = entt::resource_cache<AudioChunkResource>;
using TextureCache = entt::resource_cache<SDLTextureResource>;
using TTFFontCache = entt::resource_cache<TTFFontResource>;
using Dispatcher = entt::dispatcher;
using Registry = entt::registry<>;


using entity_type = typename Registry::entity_type;


enum class SceneType: Uint8 {
    UNKNOWN,
    EXIT,
    SPLASH_SCREEN,
    MENU_PAGE,
    INFO_PAGE,
    WALLPAPER_PAGE,
    MULTIPLAYER_PAGE,
    SETTINGS_PAGE,
    GAME_TUTORIAL,
    MULTIPLAYER_TUTORIAL,
    GAME_RECORDING_START,
    MULTIPLAYER_RECORDING_START,
    GAME_RECORDING_STOP,
    MULTIPLAYER_RECORDING_STOP,
    THE_GAME,
    MULTIPLAYER,
    MULTIPLAYER_SHARE,
    GAME_OVER,
    GAME_OVER_NO_REFRESH,
    MULTIPLAYER_RESULTS,
    TRAINING_TUTORIAL,
    TRAINING_SELECT,
    TRAINING,
    TRAINING_IS_OVER
};


enum class FaceType: Uint8 {
    ANGRY,
    DISGUSTED,
    FEARFUL,
    HAPPY,
    SAD,
    SURPRISED
};


enum class ItemType: Uint8 {
    FOUNTAIN,
    SPEED_UP,
    SLOW_DOWN,
    BOMB,
    MONEY,
    RANDOM
};


enum class UIAction: Uint8 {
    EASTER_EGG,
    EXIT,
    CLOSE,
    MENU,
    MENU_CLOSE_UP,
    MENU_CLOSE_DOWN,
    INFO,
    MULTIPLAYER,
    YOU_VS_ME,
    INVITE,
    INVITATION,
    SETTINGS,
    ACHIEVEMENTS,
    LEADERBOARD,
    THE_GAME,
    TRAINING,
    RESTART,
    SHARE,
    SHARE_WALLPAPER,
    SHARE_VIDEO,
    SWITCH_AUDIO,
    SWITCH_VIDEO,
    SWITCH_HAPTIC,
    LOGIN,
    SHOP,
    GPG_LICENSE,
    LOCKED,
    STREAM
};


enum class PanelType: Uint8 {
    EXIT,
    TITLE_TOP,
    TITLE_BOTTOM,
    SPLASH_SCREEN,
    BACKGROUND_TOP,
    BACKGROUND_BOTTOM,
    MENU_TOP,
    MENU_BOTTOM,
    INFO,
    WALLPAPER,
    MULTIPLAYER,
    SETTINGS,
    TUTORIAL_TOP,
    TUTORIAL_BOTTOM,
    THE_GAME_TOP,
    THE_GAME_BOTTOM,
    GAME_OVER,
    MULTIPLAYER_RESULTS,
    TRAINING_LEFT,
    TRAINING_RIGHT,
    TRAINING_TOP,
    TRAINING_BOTTOM,
    INVITE_TRAIN_LEFT,
    INVITE_TRAIN_RIGHT,
    INVITE_SHARE_LEFT,
    INVITE_SHARE_RIGHT,
    INVITE_SHARE_TOP,
    INVITE_SHARE_BOTTOM
};


enum class BoxBorderType: Uint8 {
    BOX_1_TOP,
    BOX_1_BOTTOM,
    BOX_1_LEFT,
    BOX_1_RIGHT,
    BOX_2_TOP,
    BOX_2_BOTTOM,
    BOX_2_LEFT,
    BOX_2_RIGHT,
    BOX_3_TOP,
    BOX_3_BOTTOM,
    BOX_3_LEFT,
    BOX_3_RIGHT,
    BOX_4_TOP,
    BOX_4_BOTTOM,
    BOX_4_LEFT,
    BOX_4_RIGHT,
    BOX_5_TOP,
    BOX_5_BOTTOM,
    BOX_5_LEFT,
    BOX_5_RIGHT,
    BOX_6_TOP,
    BOX_6_BOTTOM,
    BOX_6_LEFT,
    BOX_6_RIGHT
};


enum class PermissionType: Uint8 {
    CAMERA = 0
};


enum class PermissionStatus: Uint8 {
    DENIED = 0,
    GRANTED = 1
};


enum class AudioMusicType: Uint8 {
    AUDIO_MUSIC_UNKNOWN,
    AUDIO_MUSIC_RELAX,
    AUDIO_MUSIC_PLAY
};


enum class FaceSmashAchievement: Uint8 {
    SMASH_ME_BABY,
    LITTLE_SMASHER,
    THE_SNIPER,
    GOD_SMASHER,
    MY_FIRST_COMBO,
    COMBO_COLLECTOR_2X,
    COMBO_COLLECTOR_3X,
    COMBO_COLLECTOR_4X,
    COMBO_COLLECTOR_5X,
    SMASH_THE_ANGRY,
    SMASH_THE_DISGUSTED,
    SMASH_THE_FEARFUL,
    SMASH_THE_HAPPY,
    SMASH_THE_SAD,
    SMASH_THE_SURPRISED,
    IM_SO_HAPPY,
    BLUE_SMASH,
    NO_PAIN_NO_GAME,
    KINDERGARTEN,
    READY_TO_SMASH,
    SMASH_IS_MY_JOB,
    OH_MY_SMASH,
    SMASH_ME_CRY,
    FACE_SMASH_SUPPORTER
};


enum class Product: Uint8 {
    REMOVE_ADS
};


enum class StanzaType: Uint8 {
    IN_GAME_ITEM_TUTORIAL
};


}


#endif // FACE_SMASH_COMMON_TYPES_H
