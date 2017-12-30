#include <SDL.h>
#include <SDL_platform.h>
#include <memory>
#include <string>
#include "common/types.h"
#include "game/game_loop.h"
#include "input/user_input_handler.h"
#include "locator/locator.hpp"
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
    std::string platform = SDL_GetPlatform();

    if(platform == "Android") {
        gamee::Locator::Camera::set<gamee::CameraAndroid>();
    } else {
        gamee::Locator::Camera::set<gamee::CameraNull>();
    }

    gamee::Locator::Camera::ref().init();
}


void releasePlatformServices() {
    gamee::Locator::Camera::ref().release();
    gamee::Locator::Camera::reset();
}


int main(int , char **) {
    // set up services
    initBasicServices();
    initPlatformServices();

    // create a new game loop and initialize the environment
    auto loop = std::make_unique<gamee::GameLoop>();

    // enjoy!! :-)
    auto ret = loop->exec();

    // destroy the loop
    loop.reset();

    // tear down services
    releasePlatformServices();
    releaseBasicServices();

    // bye bye, have a nice day
    return ret;
}
