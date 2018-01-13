#include "../common/util.h"
#include "../event/event.hpp"
#include "../component/component.hpp"
#include "../locator/locator.hpp"
#include "../math/math.hpp"
#include "../service/audio_null.h"
#include "../service/audio_sdl.h"
#include "ui_button_system.h"


namespace gamee {


UIButtonSystem::UIButtonSystem(): dirty{false} {
    Locator::Dispatcher::ref().connect<TouchEvent>(this);
}


UIButtonSystem::~UIButtonSystem() {
    Locator::Dispatcher::ref().disconnect<TouchEvent>(this);
}


void UIButtonSystem::switchAudio(Registry &registry, entity_type entity) {
    auto &textureCache = Locator::TextureCache::ref();
    auto &audio = Locator::Audio::ref();

    if(audio.isMute()) {
        auto &sprite = registry.get<Sprite>(entity);
        sprite.handle = textureCache.handle("button/sound");
        Locator::Audio::set<AudioSdl>();
    } else {
        auto &sprite = registry.get<Sprite>(entity);
        sprite.handle = textureCache.handle("button/mute");
        Locator::Audio::set<AudioNull>();
    }
}


void UIButtonSystem::receive(const TouchEvent &event) noexcept {
    coord = event;
    dirty = true;
}


void UIButtonSystem::update(Registry &registry) {
    if(dirty) {
        auto view = registry.view<UIButton, Transform, BoundingBox>();

        view.each([&, this](auto entity, auto &button, auto &transform, auto &box) {
            auto area = transformToPosition(registry, entity, transform) * box;

            if(SDL_PointInRect(&coord, &area)) {
                switch(button.action) {
                case UIAction::GAME_TUTORIAL:
                    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::GAME_TUTORIAL);
                    break;
                case UIAction::RELOAD:
                case UIAction::PLAY:
                    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::THE_GAME);
                    break;
                case UIAction::MENU:
                    Locator::Dispatcher::ref().enqueue<SceneChangeEvent>(SceneType::MENU_PAGE);
                    break;
                case UIAction::SHARE:
                    // TODO
                    break;
                case UIAction::SWITCH_AUDIO:
                    switchAudio(registry, entity);
                    break;
                }
            }
        });

        dirty = false;
    }
}


}
