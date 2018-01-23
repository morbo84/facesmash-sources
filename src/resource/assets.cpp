#include <SDL_render.h>
#include "../common/constants.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "assets.h"


namespace gamee {


void loadDefaultFont(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    ttfFontCache.load<TTFFontLoader>("font/default/small", "font/04B_30__.ttf", 54);
    ttfFontCache.load<TTFFontLoader>("font/default/normal", "font/04B_30__.ttf", 72);
    ttfFontCache.load<TTFFontLoader>("font/default/large", "font/04B_30__.ttf", 90);
    ttfFontCache.load<TTFFontLoader>("font/default/huge", "font/04B_30__.ttf", 108);

    textureCache.load<TTFFontTextureLoader>("str/hud/0", "0", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/1", "1", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/2", "2", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/3", "3", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/4", "4", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/5", "5", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/6", "6", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/7", "7", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/8", "8", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/9", "9", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/ ", " ", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);
    textureCache.load<TTFFontTextureLoader>("str/hud/.", ".", renderer, *ttfFontCache.handle("font/default/small"), colorBlack);

    textureCache.load<TTFFontTextureLoader>("str/facesmash/red/F", "F", renderer, *ttfFontCache.handle("font/default/huge"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/lightblue/A", "A", renderer, *ttfFontCache.handle("font/default/huge"), colorLightBlue);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/yellow/C", "C", renderer, *ttfFontCache.handle("font/default/huge"), colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/blue/E", "E", renderer, *ttfFontCache.handle("font/default/huge"), colorBlue);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/green/S", "S", renderer, *ttfFontCache.handle("font/default/huge"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/lightblue/M", "M", renderer, *ttfFontCache.handle("font/default/huge"), colorLightBlue);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/yellow/A", "A", renderer, *ttfFontCache.handle("font/default/huge"), colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/red/S", "S", renderer, *ttfFontCache.handle("font/default/huge"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/facesmash/green/H", "H", renderer, *ttfFontCache.handle("font/default/huge"), colorGreen);
}


void loadGameStuff(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

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

    textureCache.load<TTFFontTextureLoader>("str/gamee", "GAMEE", renderer, *ttfFontCache.handle("font/default/huge"), colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/miss/10", "10", renderer, *ttfFontCache.handle("font/default/normal"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/20", "20", renderer, *ttfFontCache.handle("font/default/normal"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/50", "50", renderer, *ttfFontCache.handle("font/default/normal"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/100", "100", renderer, *ttfFontCache.handle("font/default/normal"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/smash/50", "50", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/100", "100", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/200", "200", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/250", "250", renderer, *ttfFontCache.handle("font/default/normal"), colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/reward/perfect", "PERFECT", renderer, *ttfFontCache.handle("font/default/large"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x2", "2x COMBO", renderer, *ttfFontCache.handle("font/default/large"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x3", "3x COMBO", renderer, *ttfFontCache.handle("font/default/large"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x4", "4x COMBO", renderer, *ttfFontCache.handle("font/default/large"), colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x5", "5x COMBO", renderer, *ttfFontCache.handle("font/default/large"), colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/hud/score", "SCORE", renderer, *ttfFontCache.handle("font/default/small"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/hud/timer", "TIME", renderer, *ttfFontCache.handle("font/default/small"), colorRed);

    textureCache.load<TTFFontTextureLoader>("str/tutorial/face", "USE YOUR FACE", renderer, *ttfFontCache.handle("font/default/normal"), colorRed);
    textureCache.load<TTFFontTextureLoader>("str/tutorial/touch", "USE YOUR FINGER", renderer, *ttfFontCache.handle("font/default/normal"), colorRed);

    textureCache.load<AssetTextureLoader>("palette/bg_top", "png/palette/mario/bg_top.png", renderer);
    textureCache.load<AssetTextureLoader>("palette/bg_middle", "png/palette/mario/bg_middle.png", renderer);
    textureCache.load<AssetTextureLoader>("palette/bg_bottom", "png/palette/mario/bg_bottom.png", renderer);

    textureCache.load<AssetTextureLoader>("bt/empty", "png/gui/bt_empty.png", renderer);

    textureCache.load<AssetTextureLoader>("gui/window", "png/gui/window.png", renderer);
    textureCache.load<AssetTextureLoader>("gui/popup", "png/gui/popup.png", renderer);
    textureCache.load<AssetTextureLoader>("gui/ribbon", "png/gui/ribbon.png", renderer);
}


void loadCameraFrame(GameRenderer &renderer) {
    const auto &cameraService = Locator::Camera::ref();
    Locator::TextureCache::ref().load<SDLTextureLoader>("camera/frame", renderer, internalFormat, SDL_TEXTUREACCESS_STREAMING, cameraService.width(), cameraService.height());
}


void loadDebugFont(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    ttfFontCache.load<TTFFontLoader>("font/debug/small", "font/one_constant.ttf", 54);
    ttfFontCache.load<TTFFontLoader>("font/debug/normal", "font/one_constant.ttf", 72);
    ttfFontCache.load<TTFFontLoader>("font/debug/large", "font/one_constant.ttf", 90);

    textureCache.load<TTFFontTextureLoader>("str/debug/fps", "FPS", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/time", "Time", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/debug/0", "0", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/1", "1", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/2", "2", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/3", "3", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/4", "4", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/5", "5", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/6", "6", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/7", "7", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/8", "8", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/9", "9", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/.", ".", renderer, *ttfFontCache.handle("font/debug/small"), colorWhite);
}


void loadTargetTextures(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();

    textureCache.load<SDLTextureLoader>("target/logical", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
    textureCache.load<SDLTextureLoader>("target/recording", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, recordingWidth, recordingHeight);
}


}
