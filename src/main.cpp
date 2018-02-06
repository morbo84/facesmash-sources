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
#include "service/settings_onfile.h"
#include "service/settings_onmemory.h"


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
    gamee::Locator::Audio::set<gamee::AudioSdl>();

#ifdef __ANDROID__
    gamee::Locator::Camera::set<gamee::CameraAndroid>();
    gamee::Locator::Ads::set<gamee::AdsAndroid>();
    gamee::Locator::AvRecorder::set<gamee::AvRecorderAndroid>();
    gamee::Locator::Settings::set<gamee::SettingsAndroid>();
#else
    gamee::Locator::Camera::set<gamee::CameraNull>();
    gamee::Locator::Ads::set<gamee::AdsNull>();
    gamee::Locator::AvRecorder::set<gamee::AvRecorderNull>();
    gamee::Locator::Settings::set<gamee::SettingsOnMemory>();
#endif

    gamee::Locator::Audio::ref().init();
}


void releasePlatformServices() {
    gamee::Locator::Audio::ref().release();

    gamee::Locator::AvRecorder::reset();
    gamee::Locator::Ads::reset();
    gamee::Locator::Camera::reset();
    gamee::Locator::Audio::reset();
    gamee::Locator::FaceBus::reset();
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
