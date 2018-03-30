#include <algorithm>
#include <SDL_render.h>
#include "../common/types.h"
#include "../component/component.hpp"
#include "../event/event.hpp"
#include "../factory/game_factory.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "../resource/assets.h"
#include "frame_system.h"


namespace gamee {


FrameSystem::FrameSystem() noexcept
    : refresh{false},
      dirty{false}
{
    Locator::Dispatcher::ref().connect<FrameAvailableEvent>(this);
    Locator::Dispatcher::ref().connect<PermissionEvent>(this);
}


FrameSystem::~FrameSystem() noexcept {
    Locator::Dispatcher::ref().disconnect<PermissionEvent>(this);
    Locator::Dispatcher::ref().disconnect<FrameAvailableEvent>(this);
}


void FrameSystem::receive(const FrameAvailableEvent &) noexcept {
    dirty = true;
}


void FrameSystem::receive(const PermissionEvent &event) noexcept {
    refresh = (event.permission == PermissionType::CAMERA && event.result == PermissionStatus::GRANTED);
}


void FrameSystem::update(Registry &registry, GameRenderer &renderer) {
    if(refresh) {
        registry.destroy(registry.attachee<CameraFrame>());
        prepareCameraFrame(renderer);
        createCameraFrame(registry);
        refresh = false;
    }

    const bool acquire = registry.has<CameraFrame>() &&
            registry.get<CameraFrame>().acquire;

    if(acquire && dirty) {
        auto &cameraService = Locator::Camera::ref();

        cameraService.frame([&](const void *pixels, int size) {
            auto handle = Locator::TextureCache::ref().handle("camera/frame");

            void *texture;
            int pitch;

            SDL_LockTexture(*handle, nullptr, &texture, &pitch);
            std::copy_n(static_cast<const unsigned char *>(pixels), size, static_cast<unsigned char *>(texture));
            SDL_UnlockTexture(*handle);
        });

        dirty = false;
    }
}


}
