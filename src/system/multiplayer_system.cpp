#include <algorithm>
#include <SDL_pixels.h>
#include <SDL_surface.h>
#include "../component/component.hpp"
#include "../common/util.h"
#include "../locator/locator.hpp"
#include "multiplayer_system.h"
#include <SDL_rect.h>
#include <algorithm>


namespace gamee {


void MultiplayerSystem::acquire(Registry &registry) {
    Locator::GameServices::ref().multiplayer().getOpponentFrame([](const void *pixels, int size) {
        auto handle = Locator::TextureCache::ref().handle("opponent/frame");

        void *texture;
        int pitch;

        SDL_LockTexture(*handle, nullptr, &texture, &pitch);
        std::copy_n(static_cast<const unsigned char *>(pixels), size, static_cast<unsigned char *>(texture));
        SDL_UnlockTexture(*handle);
    });

    Locator::GameServices::ref().multiplayer().getOpponentScore([&registry, this](int opponent) {
        if(current < opponent) {
            current += std::max((opponent - current) / 3, 1);
        } else if(current > opponent) {
            current -= std::max((current - opponent) / 3, 1);
        }

        registry.view<PlayerScoreObserver>().each([&, this](auto, auto &observer) {
            if(!observer.local) {
                auto &textureCache = Locator::TextureCache::ref();
                auto symEmptyHandle = textureCache.handle("str/small/ ");

                // cap the score to the limit imposed by the number of entities used to represent it
                const int cap = std::pow(10, std::extent<decltype(PlayerScoreObserver::entities)>::value);
                auto score = std::min(current, cap - 1);

                const int last = std::extent<decltype(PlayerScoreObserver::entities)>::value;
                const int offset = numOfDigits(score);

                for(auto i = offset; i < last; ++i) {
                    registry.assign_or_replace<HUD>(observer.entities[i], symEmptyHandle, symEmptyHandle->width(), symEmptyHandle->height(), symEmptyHandle->width(), symEmptyHandle->height());
                }

                for(auto i = offset; i > 0; --i) {
                    auto handle = toStrSmallHandle(score % 10);
                    registry.assign_or_replace<HUD>(observer.entities[i-1], handle, handle->width(), handle->height(), handle->width(), handle->height());
                    score /= 10;
                }
            }
        });
    });
}


MultiplayerSystem::MultiplayerSystem() {
    Locator::Dispatcher::ref().sink<SceneChangeEvent>().connect<&MultiplayerSystem::onSceneChange>(this);
    Locator::Dispatcher::ref().sink<CameraInitEvent>().connect<&MultiplayerSystem::onCameraInit>(this);
}


MultiplayerSystem::~MultiplayerSystem() {
    Locator::Dispatcher::ref().sink<CameraInitEvent>().disconnect(this);
    Locator::Dispatcher::ref().sink<SceneChangeEvent>().disconnect(this);

    SDL_FreeSurface(cpy);
    SDL_FreeSurface(buf);
}


void MultiplayerSystem::update(Registry &registry, delta_type delta) {
    if(Locator::GameServices::ref().multiplayer().readyPlayerOne()) {
        if(shareStream) {
            elapsed += delta;

            if(elapsed > share_interval) {
                acquire(registry);

                Locator::Camera::ref().frame([this](const void *pixels, int size) {
                    std::copy_n(static_cast<const unsigned char *>(pixels), size, static_cast<unsigned char *>(cpy->pixels));

                    SDL_BlitScaled(cpy, &clip, buf, nullptr);
                    Locator::GameServices::ref().multiplayer().sendCameraFrame(buf->pixels, buf->h * buf->pitch);
                });

                elapsed = {};
            }
        }

        if(!registry.empty<PlayerScore>()) {
            Locator::GameServices::ref().multiplayer().sendPlayerScore(registry.raw<PlayerScore>()->score);
        }
    } else {
        elapsed = {};
        current = {};
    }
}


void MultiplayerSystem::onSceneChange(const SceneChangeEvent &event) noexcept {
    shareStream = event.scene == SceneType::MULTIPLAYER_SHARE;
}


void MultiplayerSystem::onCameraInit(const CameraInitEvent &) noexcept {
    const auto &camera = Locator::Camera::ref();
    const auto width = camera.width();
    const auto height = camera.height();

    cpy = SDL_CreateRGBSurfaceWithFormat(0, width, height, SDL_BITSPERPIXEL(internalFormat), internalFormat);
    buf = SDL_CreateRGBSurfaceWithFormat(0, opponentFrameWidth, opponentFrameHeight, SDL_BITSPERPIXEL(internalFormat), internalFormat);

    int mul{};

    while(((mul+1) * opponentFrameWidth) < width && ((mul+1) * opponentFrameHeight) < height) {
        ++mul;
    }

    const auto w = mul * opponentFrameWidth;
    const auto h = mul * opponentFrameHeight;

    clip.x = (width - w) / 2;
    clip.y = (height - h) / 2;
    clip.w = w;
    clip.h = h;
}


}
