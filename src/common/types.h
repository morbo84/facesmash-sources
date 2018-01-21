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
    SPLASH_SCREEN,
    MENU_PAGE,
    CREDITS_PAGE,
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
    GAME_TUTORIAL,
    TRAINING_TUTORIAL,
    RELOAD,
    SHARE,
    SWITCH_AUDIO
};


enum class PanelType: Uint8 {
    SPLASH_SCREEN,
    BACKGROUND_TOP_PANEL,
    BACKGROUND_BOTTOM_PANEL,
    MENU_TOP_PANEL,
    MENU_BOTTOM_PANEL,
    CREDITS_PANEL,
    TUTORIAL_TOP_PANEL,
    TUTORIAL_BOTTOM_PANEL,
    GAME_TOP_PANEL,
    GAME_BOTTOM_PANEL,
    GAME_OVER_PANEL,
    TRAINING_TOP_PANEL,
    TRAINING_BOTTOM_PANEL,
    SMASH_BUTTONS_PANEL
};


}


#endif // FACE_SMASH_COMMON_TYPES_H
