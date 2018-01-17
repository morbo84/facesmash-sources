#ifndef FACE_SMASH_LOCATOR_LOCATOR_HPP
#define FACE_SMASH_LOCATOR_LOCATOR_HPP


#include <entt/locator/locator.hpp>
#include "../common/types.h"
#include "../input/input_handler.h"
#include "../service/ads_service.h"
#include "../service/audio_service.h"
#include "../service/av_muxer_service.h"
#include "../service/camera_service.h"
#include "../service/face_bus_service.h"


namespace gamee {


struct Locator final {
    // from entt with love
    using TTFFontCache = entt::ServiceLocator<gamee::TTFFontCache>;
    using TextureCache = entt::ServiceLocator<gamee::TextureCache>;
    using Dispatcher = entt::ServiceLocator<gamee::Dispatcher>;
    using InputHandler = entt::ServiceLocator<gamee::InputHandler>;
    // actual services
    using Ads = entt::ServiceLocator<gamee::AdsService>;
    using Camera = entt::ServiceLocator<gamee::CameraService>;
    using Audio = entt::ServiceLocator<gamee::AudioService>;
    using FaceBus = entt::ServiceLocator<gamee::FaceBusService>;
    using AvMuxer = entt::ServiceLocator<gamee::AvMuxerService>;
};


}


#endif // FACE_SMASH_LOCATOR_LOCATOR_HPP
