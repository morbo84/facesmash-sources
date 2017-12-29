#ifndef FACE_SMASH_LOCATOR_LOCATOR_HPP
#define FACE_SMASH_LOCATOR_LOCATOR_HPP


#include <entt/locator/locator.hpp>
#include "../common/types.h"
#include "../input/input_handler.h"


namespace gamee {


struct Locator final {
    using TTFFontCache = entt::ServiceLocator<gamee::TTFFontCache>;
    using TextureCache = entt::ServiceLocator<gamee::TextureCache>;
    using Dispatcher = entt::ServiceLocator<gamee::Dispatcher>;
    using InputHandler = entt::ServiceLocator<gamee::InputHandler>;
};


}


#endif // FACE_SMASH_LOCATOR_LOCATOR_HPP
