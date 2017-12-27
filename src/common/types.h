#ifndef FACE_SMASH_COMMON_TYPES_H
#define FACE_SMASH_COMMON_TYPES_H


#include <cstdint>
#include <SDL_stdinc.h>
#include <entt/entity/registry.hpp>
#include <entt/process/process.hpp>
#include <entt/process/scheduler.hpp>
#include <entt/resource/cache.hpp>
#include <entt/signal/dispatcher.hpp>
#include "../resource/font_resource.h"
#include "../resource/texture_resource.h"


namespace gamee {


using UInt8 = Uint8;
using SInt8 = Sint8;
using UInt16 = Uint16;
using SInt16 = Sint16;
using UInt32 = Uint32;
using SInt32 = Sint32;
using UInt64 = Uint64;
using SInt64 = Sint64;


constexpr UInt8 operator"" _ui8 (unsigned long long ull) { return UInt8(ull); }
constexpr SInt8 operator"" _si8 (unsigned long long ull) { return SInt8(ull); }
constexpr UInt16 operator"" _ui16 (unsigned long long ull) { return UInt16(ull); }
constexpr SInt16 operator"" _si16 (unsigned long long ull) { return SInt16(ull); }
constexpr UInt32 operator"" _ui32 (unsigned long long ull) { return UInt32(ull); }
constexpr SInt32 operator"" _si32 (unsigned long long ull) { return SInt32(ull); }
constexpr UInt64 operator"" _ui64 (unsigned long long ull) { return UInt64(ull); }
constexpr SInt64 operator"" _si64 (unsigned long long ull) { return SInt64(ull); }


using delta_type = UInt32;

template<typename Derived>
using Process = entt::Process<Derived, delta_type>;

using Scheduler = entt::Scheduler<delta_type>;
using TextureCache = entt::ResourceCache<SDLTextureResource>;
using TTFFontCache = entt::ResourceCache<TTFFontResource>;
using TTFFontTextureCache = entt::ResourceCache<TTFFontTextureResource>;
using Dispatcher = entt::UnmanagedDispatcher;
using Registry = entt::DefaultRegistry;


using entity_type = typename Registry::entity_type;


enum SmashType {
    ANGRY = 0,
    DISGUSTED,
    HAPPY,
    RESTED,
    SURPRISED,
    FEARFUL,
    SAD
};


}


#endif // FACE_SMASH_COMMON_TYPES_H
