#ifndef FACE_SMASH_LOCATOR_LOCATOR_HPP
#define FACE_SMASH_LOCATOR_LOCATOR_HPP


#include <entt/locator/locator.hpp>
#include "../common/types.h"
#include "../input/input_handler.h"
#include "../service/ads_service.h"
#include "../service/audio_service.h"
#include "../service/av_recorder_service.h"
#include "../service/billing_service.h"
#include "../service/camera_service.h"
#include "../service/face_bus_service.h"
#include "../service/game_services.h"
#include "../service/haptic_service.h"
#include "../service/permissions_service.h"
#include "../service/settings_service.h"


namespace gamee {


struct Locator final {
    // from entt with love
    using AudioMusicCache = entt::service_locator<gamee::AudioMusicCache>;
    using AudioChunkCache = entt::service_locator<gamee::AudioChunkCache>;
    using TextureCache = entt::service_locator<gamee::TextureCache>;
    using TTFFontCache = entt::service_locator<gamee::TTFFontCache>;
    using Dispatcher = entt::service_locator<gamee::Dispatcher>;
    using InputHandler = entt::service_locator<gamee::InputHandler>;
    // actual services
    using GameServices = entt::service_locator<gamee::GameServicesService>;
    using Ads = entt::service_locator<gamee::AdsService>;
    using Camera = entt::service_locator<gamee::CameraService>;
    using Audio = entt::service_locator<gamee::AudioService>;
    using Haptic = entt::service_locator<gamee::HapticService>;
    using FaceBus = entt::service_locator<gamee::FaceBusService>;
    using AvRecorder = entt::service_locator<gamee::AvRecorderService>;
    using Billing = entt::service_locator<gamee::BillingService>;
    using Permissions = entt::service_locator<gamee::PermissionsService>;
    using Settings = entt::service_locator<gamee::SettingsService>;
};


}


#endif // FACE_SMASH_LOCATOR_LOCATOR_HPP
