#ifndef FACE_SMASH_COMMON_TYPES_H
#define FACE_SMASH_COMMON_TYPES_H


#include <cstdint>
#include <SDL_stdinc.h>
#include <entt/entity/registry.hpp>
#include <entt/resource/cache.hpp>
#include <entt/signal/dispatcher.hpp>
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

using TextureCache = entt::ResourceCache<SDLTextureResource>;
using TTFFontCache = entt::ResourceCache<TTFFontResource>;
using Dispatcher = entt::UnmanagedDispatcher;
using Registry = entt::DefaultRegistry;


using entity_type = typename Registry::entity_type;


enum class SceneType: Uint8 {
    UNKNOWN,
    EXIT,
    SPLASH_SCREEN,
    MENU_PAGE,
    CREDITS_PAGE,
    SUPPORT_PAGE,
    SETTINGS_PAGE,
    ACHIEVEMENTS_PAGE,
    GAME_TUTORIAL,
    THE_GAME,
    GAME_OVER,
    TRAINING_TUTORIAL,
    TRAINING
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
    SLOW_DOWN
};


enum class UIAction: Uint8 {
    EXIT,
    MENU,
    CREDITS,
    SUPPORT,
    SETTINGS,
    ACHIEVEMENTS,
    THE_GAME,
    TRAINING,
    RELOAD,
    SAVE,
    SWITCH_AUDIO,
    SWITCH_VIDEO
};


enum class PanelType: Uint8 {
    EXIT,
    SPLASH_SCREEN,
    BACKGROUND_TOP,
    BACKGROUND_BOTTOM,
    MENU_TOP,
    MENU_BOTTOM,
    CREDITS,
    SUPPORT,
    SETTINGS,
    ACHIEVEMENTS,
    TUTORIAL_TOP,
    TUTORIAL_BOTTOM,
    THE_GAME_TOP,
    THE_GAME_BOTTOM,
    GAME_OVER,
    TRAINING_TOP,
    TRAINING_BOTTOM,
    SMASH_BUTTONS
};


}


#endif // FACE_SMASH_COMMON_TYPES_H
