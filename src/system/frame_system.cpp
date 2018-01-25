#include <algorithm>
#include <SDL_render.h>
#include "../common/types.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../locator/locator.hpp"
#include "frame_system.h"


namespace gamee {


FrameSystem::FrameSystem() noexcept
    : dirty{false}
{
    Locator::Dispatcher::ref().connect<FrameAvailableEvent>(this);
}


FrameSystem::~FrameSystem() noexcept {
    Locator::Dispatcher::ref().disconnect<FrameAvailableEvent>(this);
}


void FrameSystem::receive(const FrameAvailableEvent &) noexcept {
    dirty = true;
}


void FrameSystem::update(Registry &registry) {
    const bool acquire = registry.has<CameraFrame>() &&
            registry.get<CameraFrame>().acquire;

    if(acquire && dirty) {
        auto &cameraService = Locator::Camera::ref();

        cameraService.frame([&](const void *pixels, int size) {
            auto handle = Locator::TextureCache::ref().handle("camera/frame");

            void *texture;
            int pitch;

            SDL_LockTexture(*handle, nullptr, &texture, &pitch);
            std::copy_n(static_cast<const unsigned char*>(pixels), size, static_cast<unsigned char*>(texture));
            SDL_UnlockTexture(*handle);
        });

        dirty = false;
    }
}


}
