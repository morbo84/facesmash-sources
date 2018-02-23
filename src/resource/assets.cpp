#include <SDL_render.h>
#include "../common/constants.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "assets.h"


namespace gamee {


void loadResources(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    ttfFontCache.load<TTFFontLoader>("font/ui/tiny", "font/04B_30__.ttf", 32);
    ttfFontCache.load<TTFFontLoader>("font/ui/small", "font/04B_30__.ttf", 40);
    ttfFontCache.load<TTFFontLoader>("font/ui/normal", "font/04B_30__.ttf", 56);
    ttfFontCache.load<TTFFontLoader>("font/ui/large", "font/04B_30__.ttf", 80);
    ttfFontCache.load<TTFFontLoader>("font/ui/huge", "font/04B_30__.ttf", 100);

    textureCache.load<AssetTextureLoader>("ui/buttons", "ui/buttons.png", renderer);
    textureCache.load<AssetTextureLoader>("ui/win_hud", "ui/win_hud.png", renderer);

    textureCache.load<AssetTextureLoader>("game/explosion", "other/explosion.png", renderer);
    textureCache.load<AssetTextureLoader>("game/emoji", "emoji/emoji.png", renderer);
    textureCache.load<AssetTextureLoader>("game/items", "emoji/items.png", renderer);
}


void loadDebugResources() {
    auto &ttfFontCache = Locator::TTFFontCache::ref();

    ttfFontCache.load<TTFFontLoader>("font/debug/tiny", "font/04B_03__.ttf", 32);
}


void prepareGameStrings(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    auto fontTiny = ttfFontCache.handle("font/ui/tiny");
    auto fontSmall = ttfFontCache.handle("font/ui/small");
    auto fontNormal = ttfFontCache.handle("font/ui/normal");
    auto fontLarge = ttfFontCache.handle("font/ui/large");
    auto fontHuge = ttfFontCache.handle("font/ui/huge");

    textureCache.load<TTFFontTextureLoader>("str/normal/0", "0", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/1", "1", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/2", "2", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/3", "3", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/4", "4", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/5", "5", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/6", "6", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/7", "7", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/8", "8", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/normal/9", "9", renderer, *fontNormal, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/small/0", "0", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/1", "1", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/2", "2", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/3", "3", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/4", "4", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/5", "5", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/6", "6", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/7", "7", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/8", "8", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/9", "9", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/ ", " ", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/.", ".", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/small/x", "x", renderer, *fontSmall, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/game/score", "SCORE", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/game/timer", "TIME", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/training/smash", "SMASH", renderer, *fontSmall, colorRed);

    textureCache.load<TTFFontTextureLoader>("str/score", "SCORE", renderer, *fontNormal, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/gamee", "GAMEE", renderer, *fontHuge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/face", "FACE", renderer, *fontHuge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/smash", "SMASH", renderer, *fontHuge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/exit", "Are you sure?", renderer, *fontNormal, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/credits", "CREDITS", renderer, *fontNormal, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/support", "SUPPORT US", renderer, *fontNormal, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/settings", "SETTINGS", renderer, *fontNormal, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/achievements", "ACHIEVEMENTS", renderer, *fontNormal, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/gameover", "GAME OVER", renderer, *fontLarge, colorRed);

    textureCache.load<TTFFontTextureLoader>("str/support/making", "Making games", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/support/time", "in the free time", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/support/lot", "requires lot of", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/support/coffee", "COFFEE", renderer, *fontHuge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/support/offer", "Offer us", renderer, *fontLarge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/support/next", "the next one", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/support/remove", "REMOVE ADS", renderer, *fontTiny, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/audio", "Audio", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/video", "Video", renderer, *fontSmall, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/miss/10", "10", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/20", "20", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/50", "50", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/100", "100", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/smash/50", "50", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/100", "100", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/200", "200", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/250", "250", renderer, *fontSmall, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/reward/x2", "2x COMBO", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x3", "3x COMBO", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x4", "4x COMBO", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x5", "5x COMBO", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/perfect", "PERFECT", renderer, *fontLarge, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/tutorial/face", "USE YOUR FACE", renderer, *fontNormal, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/tutorial/touch", "TOUCH THESE", renderer, *fontNormal, colorGreen);
}


void prepareDebugStrings(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();

    auto fontTiny = ttfFontCache.handle("font/debug/tiny");

    textureCache.load<TTFFontTextureLoader>("str/debug/0", "0", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/1", "1", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/2", "2", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/3", "3", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/4", "4", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/5", "5", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/6", "6", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/7", "7", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/8", "8", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/9", "9", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/ ", " ", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/.", ".", renderer, *fontTiny, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/debug/fps", "FPS", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/debug/time", "Time", renderer, *fontTiny, colorWhite);
}


void prepareGameStuff(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();

    textureCache.load<SDLTextureLoader>("target/logical", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
    textureCache.load<SDLTextureLoader>("target/recording", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, recordingWidth, recordingHeight);

    const auto &cameraService = Locator::Camera::ref();
    textureCache.load<SDLTextureLoader>("camera/frame", renderer, internalFormat, SDL_TEXTUREACCESS_STREAMING, cameraService.width(), cameraService.height());
}


}
