#ifndef FACE_SMASH_LOCATOR_LOCATOR_HPP
#define FACE_SMASH_LOCATOR_LOCATOR_HPP


#include <entt/locator/locator.hpp>
#include "../common/types.h"
#include "../input/input_handler.h"
#include "../service/ads_service.h"
#include "../service/audio_service.h"
#include "../service/av_recorder_service.h"
#include "../service/camera_service.h"
#include "../service/face_bus_service.h"
#include "../service/game_services_service.h"
#include "../service/permissions_service.h"
#include "../service/settings_service.h"


namespace gamee {


struct Locator final {
    // from entt with love
    using AudioMusicCache = entt::ServiceLocator<gamee::AudioMusicCache>;
    using AudioChunkCache = entt::ServiceLocator<gamee::AudioChunkCache>;
    using TextureCache = entt::ServiceLocator<gamee::TextureCache>;
    using TTFFontCache = entt::ServiceLocator<gamee::TTFFontCache>;
    using Dispatcher = entt::ServiceLocator<gamee::Dispatcher>;
    using InputHandler = entt::ServiceLocator<gamee::InputHandler>;
    // actual services
    using GameServices = entt::ServiceLocator<gamee::GameServicesService>;
    using Ads = entt::ServiceLocator<gamee::AdsService>;
    using Camera = entt::ServiceLocator<gamee::CameraService>;
    using Audio = entt::ServiceLocator<gamee::AudioService>;
    using FaceBus = entt::ServiceLocator<gamee::FaceBusService>;
    using AvRecorder = entt::ServiceLocator<gamee::AvRecorderService>;
    using Permissions = entt::ServiceLocator<gamee::PermissionsService>;
    using Settings = entt::ServiceLocator<gamee::SettingsService>;
};


}


#endif // FACE_SMASH_LOCATOR_LOCATOR_HPP
