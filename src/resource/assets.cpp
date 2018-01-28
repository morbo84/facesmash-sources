#include <SDL_render.h>
#include "../common/constants.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "assets.h"


namespace gamee {


void loadDefaultFont() {
    auto &ttfFontCache = Locator::TTFFontCache::ref();

    ttfFontCache.load<TTFFontLoader>("font/default/tiny", "font/04B_30__.ttf", 36);
    ttfFontCache.load<TTFFontLoader>("font/default/small", "font/04B_30__.ttf", 54);
    ttfFontCache.load<TTFFontLoader>("font/default/normal", "font/04B_30__.ttf", 72);
    ttfFontCache.load<TTFFontLoader>("font/default/large", "font/04B_30__.ttf", 90);
    ttfFontCache.load<TTFFontLoader>("font/default/huge", "font/04B_30__.ttf", 108);
}


void loadDebugFont() {
    Locator::TTFFontCache::ref().load<TTFFontLoader>("font/debug", "font/one_constant.ttf", 36);
}


void loadGameStuff(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    textureCache.load<TTFFontTextureLoader>("str/hud/0", "0", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/1", "1", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/2", "2", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/3", "3", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/4", "4", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/5", "5", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/6", "6", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/7", "7", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/8", "8", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/9", "9", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/ ", " ", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/.", ".", renderer, *ttfFontCache.handle("font/default/tiny"), colorBlack);

    textureCache.load<TTFFontTextureLoader>("str/gamee", "GAMEE", renderer, *ttfFontCache.handle("font/default/large"), colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/facesmash/red/F", "F", renderer, *ttfFontCache.handle("font/default/huge"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/lightblue/A", "A", renderer, *ttfFontCache.handle("font/default/huge"), colorLightBlue);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/yellow/C", "C", renderer, *ttfFontCache.handle("font/default/huge"), colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/blue/E", "E", renderer, *ttfFontCache.handle("font/default/huge"), colorBlue);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/green/S", "S", renderer, *ttfFontCache.handle("font/default/huge"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/lightblue/M", "M", renderer, *ttfFontCache.handle("font/default/huge"), colorLightBlue);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/yellow/A", "A", renderer, *ttfFontCache.handle("font/default/huge"), colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/red/S", "S", renderer, *ttfFontCache.handle("font/default/huge"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/green/H", "H", renderer, *ttfFontCache.handle("font/default/huge"), colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/credits", "CREDITS", renderer, *ttfFontCache.handle("font/default/normal"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/support", "SUPPORT US", renderer, *ttfFontCache.handle("font/default/normal"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/settings", "SETTINGS", renderer, *ttfFontCache.handle("font/default/normal"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/achievements", "ACHIEVEMENTS", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/gameover", "GAME OVER", renderer, *ttfFontCache.handle("font/default/normal"), colorBlack);

    textureCache.load<TTFFontTextureLoader>("str/audio", "Audio", renderer, *ttfFontCache.handle("font/default/normal"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/video", "Video", renderer, *ttfFontCache.handle("font/default/normal"), colorBlack);

    textureCache.load<AssetTextureLoader>("bt/small", "png/gui/bt_small.png", renderer);
    textureCache.load<AssetTextureLoader>("bt/normal", "png/gui/bt_normal.png", renderer);
    textureCache.load<AssetTextureLoader>("bt/large", "png/gui/bt_large.png", renderer);

    textureCache.load<AssetTextureLoader>("img/play", "png/gui/gamepad.png", renderer);
    textureCache.load<AssetTextureLoader>("img/training", "png/gui/target.png", renderer);
    textureCache.load<AssetTextureLoader>("img/save", "png/gui/save.png", renderer);
    textureCache.load<AssetTextureLoader>("img/menu", "png/gui/menu.png", renderer);
    textureCache.load<AssetTextureLoader>("img/gear", "png/gui/gear.png", renderer);
    textureCache.load<AssetTextureLoader>("img/medal", "png/gui/medal.png", renderer);
    textureCache.load<AssetTextureLoader>("img/info", "png/gui/information.png", renderer);
    textureCache.load<AssetTextureLoader>("img/support", "png/gui/support.png", renderer);
    textureCache.load<AssetTextureLoader>("img/back", "png/gui/back.png", renderer);
    textureCache.load<AssetTextureLoader>("img/prev", "png/gui/prev.png", renderer);
    textureCache.load<AssetTextureLoader>("img/next", "png/gui/next.png", renderer);

    textureCache.load<AssetTextureLoader>("img/audio/on", "png/gui/audio_on.png", renderer);
    textureCache.load<AssetTextureLoader>("img/audio/off", "png/gui/audio_off.png", renderer);
    textureCache.load<AssetTextureLoader>("img/video/on", "png/gui/video_on.png", renderer);
    textureCache.load<AssetTextureLoader>("img/video/off", "png/gui/video_off.png", renderer);

    textureCache.load<AssetTextureLoader>("game/explosion", "png/gui/explosion.png", renderer);

    textureCache.load<AssetTextureLoader>("face/angry", "png/emoji/emojitwo/angry.png", renderer);
    textureCache.load<AssetTextureLoader>("face/disgusted", "png/emoji/emojitwo/disgusted.png", renderer);
    textureCache.load<AssetTextureLoader>("face/fearful", "png/emoji/emojitwo/fearful.png", renderer);
    textureCache.load<AssetTextureLoader>("face/happy", "png/emoji/emojitwo/happy.png", renderer);
    textureCache.load<AssetTextureLoader>("face/sad", "png/emoji/emojitwo/sad.png", renderer);
    textureCache.load<AssetTextureLoader>("face/surprised", "png/emoji/emojitwo/surprised.png", renderer);

    textureCache.load<AssetTextureLoader>("item/speed_up", "png/emoji/emojitwo/fire.png", renderer);
    textureCache.load<AssetTextureLoader>("item/slow_down", "png/emoji/emojitwo/hourglass.png", renderer);
    textureCache.load<AssetTextureLoader>("item/fountain", "png/emoji/emojitwo/pill.png", renderer);

    textureCache.load<TTFFontTextureLoader>("str/miss/10", "10", renderer, *ttfFontCache.handle("font/default/small"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/20", "20", renderer, *ttfFontCache.handle("font/default/small"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/50", "50", renderer, *ttfFontCache.handle("font/default/small"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/100", "100", renderer, *ttfFontCache.handle("font/default/small"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/smash/50", "50", renderer, *ttfFontCache.handle("font/default/small"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/100", "100", renderer, *ttfFontCache.handle("font/default/small"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/200", "200", renderer, *ttfFontCache.handle("font/default/small"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/250", "250", renderer, *ttfFontCache.handle("font/default/small"), colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/reward/perfect", "PERFECT", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x2", "2x COMBO", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x3", "3x COMBO", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x4", "4x COMBO", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x5", "5x COMBO", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/hud/score", "SCORE", renderer, *ttfFontCache.handle("font/default/tiny"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/hud/timer", "TIME", renderer, *ttfFontCache.handle("font/default/tiny"), colorRed);

    textureCache.load<TTFFontTextureLoader>("str/tutorial/face", "USE YOUR FACE", renderer, *ttfFontCache.handle("font/default/small"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/tutorial/touch", "TOUCH THESE", renderer, *ttfFontCache.handle("font/default/small"), colorRed);
}


void loadCameraFrame(GameRenderer &renderer) {
    const auto &cameraService = Locator::Camera::ref();
    Locator::TextureCache::ref().load<SDLTextureLoader>("camera/frame", renderer, internalFormat, SDL_TEXTUREACCESS_STREAMING, cameraService.width(), cameraService.height());
}


void loadDebugStuff(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    textureCache.load<TTFFontTextureLoader>("str/debug/fps", "FPS", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/time", "Time", renderer, *ttfFontCache.handle("font/debug"), colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/debug/0", "0", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/1", "1", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/2", "2", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/3", "3", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/4", "4", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/5", "5", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/6", "6", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/7", "7", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/8", "8", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/9", "9", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/.", ".", renderer, *ttfFontCache.handle("font/debug"), colorWhite);
}


void loadTargetTextures(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();

    textureCache.load<SDLTextureLoader>("target/logical", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
    textureCache.load<SDLTextureLoader>("target/recording", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, recordingWidth, recordingHeight);
}


}
