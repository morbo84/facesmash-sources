#include <SDL.h>
#include <memory>
#include <string>
#include "common/types.h"
#include "emotion/emo_detector.h"
#include "game/game_loop.h"
#include "input/user_input_handler.h"
#include "locator/locator.hpp"
#include "service/audio_null.h"
#include "service/audio_sdl.h"
#include "service/camera_android.h"
#include "service/camera_null.h"


void initBasicServices() {
    gamee::Locator::TTFFontCache::set();
    gamee::Locator::TextureCache::set();
    gamee::Locator::Dispatcher::set();
    gamee::Locator::InputHandler::set<gamee::UserInputHandler>();
}


void releaseBasicServices() {
    gamee::Locator::InputHandler::reset();
    gamee::Locator::Dispatcher::reset();
    gamee::Locator::TextureCache::reset();
    gamee::Locator::TTFFontCache::reset();
}


void initPlatformServices() {
    gamee::Locator::FaceBus::set<gamee::FaceBusService>();
    gamee::Locator::Audio::set<gamee::AudioNull>();

#ifdef __ANDROID__
    gamee::Locator::Camera::set<gamee::CameraAndroid>();
#else
    gamee::Locator::Camera::set<gamee::CameraNull>();
#endif

    gamee::Locator::Audio::ref().init();
}


void releasePlatformServices() {
    gamee::Locator::Audio::ref().release();

    gamee::Locator::Camera::reset();
    gamee::Locator::Audio::reset();
    gamee::Locator::FaceBus::reset();
}

int main(int , char **) {
    // set up services
    initBasicServices();
    initPlatformServices();

    const auto &camera = gamee::Locator::Camera::ref();
    // initialize the emo detector
    auto emoDetector = std::make_unique<gamee::EmoDetector>(camera.width(), camera.height());

    // create a new game loop and initialize the environment
    auto loop = std::make_unique<gamee::GameLoop>();

    // enjoy!! :-)
    auto ret = loop->exec();

    // destroy the loop
    loop.reset();

    // reset the emo detector before to shutdown all the other services
    emoDetector.reset();

    // tear down services
    releasePlatformServices();
    releaseBasicServices();

    // bye bye, have a nice day
    return ret;
}
