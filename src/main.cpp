#include <SDL.h>
#include <memory>
#include <string>
#include "common/types.h"
#include "emotion/emo_detector.h"
#include "game/game_loop.h"
#include "input/user_input_handler.h"
#include "locator/locator.hpp"
#include "service/ads_android.h"
#include "service/ads_null.h"
#include "service/audio_null.h"
#include "service/audio_sdl.h"
#include "service/av_recorder_android.h"
#include "service/av_recorder_null.h"
#include "service/camera_android.h"
#include "service/camera_null.h"
#include "service/game_services_android.h"
#include "service/game_services_null.h"
#include "service/haptic_null.h"
#include "service/haptic_sdl.h"
#include "service/permissions_null.h"
#include "service/permissions_android.h"
#include "service/settings_onfile.h"
#include "service/settings_onmemory.h"


static void initBasicServices() {
    gamee::Locator::TTFFontCache::set();
    gamee::Locator::TextureCache::set();
    gamee::Locator::Dispatcher::set();
    gamee::Locator::InputHandler::set<gamee::UserInputHandler>();
}


static void releaseBasicServices() {
    gamee::Locator::InputHandler::reset();
    gamee::Locator::Dispatcher::reset();
    gamee::Locator::TextureCache::reset();
    gamee::Locator::TTFFontCache::reset();
}


static void initPlatformServices() {
#ifdef __ANDROID__
    gamee::Locator::Permissions::set<gamee::PermissionsAndroid>();
    gamee::Locator::Settings::set<gamee::SettingsOnFile>();
    gamee::Locator::Camera::set<gamee::CameraAndroid>();
    gamee::Locator::GameServices::set<gamee::GameServicesAndroid>();
    gamee::Locator::Ads::set<gamee::AdsAndroid>();
#else
    gamee::Locator::Permissions::set<gamee::PermissionsNull>();
    gamee::Locator::Settings::set<gamee::SettingsOnMemory>();
    gamee::Locator::Camera::set<gamee::CameraNull>();
    gamee::Locator::GameServices::set<gamee::GameServicesNull>();
    gamee::Locator::Ads::set<gamee::AdsNull>();
#endif

#ifdef DEBUG
    // turn off ads in debug mode, no matter if we are on android
    gamee::Locator::Ads::set<gamee::AdsNull>();
#endif

    gamee::Locator::AvRecorder::set<gamee::AvRecorderNull>();
    gamee::Locator::FaceBus::set<gamee::FaceBusService>();
    gamee::Locator::Audio::set<gamee::AudioNull>();
    gamee::Locator::Haptic::set<gamee::HapticNull>();
}


static void releasePlatformServices() {
    gamee::Locator::Haptic::reset();
    gamee::Locator::Audio::reset();
    gamee::Locator::FaceBus::reset();
    gamee::Locator::AvRecorder::reset();
    gamee::Locator::Ads::reset();
    gamee::Locator::GameServices::reset();
    gamee::Locator::Camera::reset();
    gamee::Locator::Settings::reset();
    gamee::Locator::Permissions::reset();
}


static void readSettings() {
    auto &settings = gamee::Locator::Settings::ref();

#ifdef __ANDROID__
    if(settings.read("video/available", true)) {
        gamee::Locator::AvRecorder::set<gamee::AvRecorderAndroid>();
    }
#else
    settings.write("video/available", false);
#endif

    if(settings.read("audio/available", true)) {
        gamee::Locator::Audio::set<gamee::AudioSDL>();
    }

    if(settings.read("haptic/available", true)) {
        gamee::Locator::Haptic::set<gamee::HapticSDL>();
    }
}


int main(int, char **) {
    // set up services
    initBasicServices();
    initPlatformServices();

    const auto &camera = gamee::Locator::Camera::ref();
    // initialize the emo detector
    auto emoDetector = std::make_unique<gamee::EmoDetector>(camera.width(), camera.height());

    // create a new game loop and initialize the environment
    auto loop = std::make_unique<gamee::GameLoop>();

    // read settings and adjust services if required
    readSettings();

    // try to log in for fun and profit
    gamee::Locator::GameServices::ref().signIn();

    // enjoy!! :-)
    auto ret = loop->exec();

    // tear down standard services
    releasePlatformServices();

    // destroy the loop
    loop.reset();

    // reset the emo detector before to shutdown all the other services
    emoDetector.reset();

    // tear down standard services
    releaseBasicServices();

    // bye bye, have a nice day
    return ret;
}
