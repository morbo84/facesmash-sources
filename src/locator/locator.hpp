#ifndef FACE_SMASH_LOCATOR_LOCATOR_HPP
#define FACE_SMASH_LOCATOR_LOCATOR_HPP


#include <entt/locator/locator.hpp>
#include "../common/types.h"
#include "../input/input_handler.h"
#include "../service/camera_service.h"


namespace gamee {


struct Locator final {
    using TTFFontCache = entt::ServiceLocator<gamee::TTFFontCache>;
    using TextureCache = entt::ServiceLocator<gamee::TextureCache>;
    using Dispatcher = entt::ServiceLocator<gamee::Dispatcher>;
    using InputHandler = entt::ServiceLocator<gamee::InputHandler>;
    using Camera = entt::ServiceLocator<gamee::CameraService>;
};


}


#endif // FACE_SMASH_LOCATOR_LOCATOR_HPP
