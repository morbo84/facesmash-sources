#include <SDL_render.h>
#include "../common/constants.h"
#include "../game/game_renderer.h"
#include "../locator/locator.hpp"
#include "assets.h"


namespace gamee {


void loadResources(GameRenderer &renderer) {
    auto &ttfFontCache = Locator::TTFFontCache::ref();
    auto &textureCache = Locator::TextureCache::ref();
    auto &musicCache = Locator::AudioMusicCache::ref();

    ttfFontCache.load<TTFFontLoader>("font/ui/tiny", "font/04B_30__.ttf", 32);
    ttfFontCache.load<TTFFontLoader>("font/ui/small", "font/04B_30__.ttf", 40);
    ttfFontCache.load<TTFFontLoader>("font/ui/normal", "font/04B_30__.ttf", 56);
    ttfFontCache.load<TTFFontLoader>("font/ui/large", "font/04B_30__.ttf", 80);
    ttfFontCache.load<TTFFontLoader>("font/ui/huge", "font/04B_30__.ttf", 104);

    textureCache.load<AssetTextureLoader>("ui/buttons", "ui/buttons.png", renderer);
    textureCache.load<AssetTextureLoader>("ui/win_hud", "ui/win_hud.png", renderer);

    textureCache.load<AssetTextureLoader>("game/explosion", "other/explosion.png", renderer);
    textureCache.load<AssetTextureLoader>("game/emoji", "emoji/emoji.png", renderer);

    musicCache.load<AudioMusicLoader>("music/relax", "audio/music_relax.ogg");
    musicCache.load<AudioMusicLoader>("music/play", "audio/music_play.ogg");
}


void loadDebugResources() {
    Locator::TTFFontCache::ref().load<TTFFontLoader>("font/debug/tiny", "font/04B_03__.ttf", 32);
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

    textureCache.load<TTFFontTextureLoader>("str/share/facesmashchallenge", "#FaceSmashChallenge", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/share/facesmash", "#FaceSmash", renderer, *fontSmall, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/item/fountain", "COME ON", renderer, *fontLarge, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/item/slow", "KEEP CALM", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/item/hurry", "LET'S RUN", renderer, *fontLarge, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/item/bomb", "WOOP", renderer, *fontLarge, colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/item/money", "HOORAY", renderer, *fontLarge, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/score", "SCORE", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/vs", "VS", renderer, *fontNormal, colorRed);

    textureCache.load<TTFFontTextureLoader>("str/won", "YOU WON", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/lose", "YOU LOSE", renderer, *fontLarge, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/draw", "DRAW", renderer, *fontLarge, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/gamee", "GAMEE", renderer, *fontHuge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/face", "FACE", renderer, *fontHuge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/smash", "SMASH", renderer, *fontHuge, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/exit", "Are you sure?", renderer, *fontNormal, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/training", "Training", renderer, *fontLarge, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/info", "About", renderer, *fontLarge, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/settings", "Settings", renderer, *fontLarge, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/challenges", "Challenges", renderer, *fontLarge, colorRed);

    textureCache.load<TTFFontTextureLoader>("str/gameover", "Game Over", renderer, *fontLarge, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/gameover/training", "Need to TRAIN?", renderer, *fontSmall, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/multiplayer/share", "Share your FACE", renderer, *fontSmall, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/emotion/angry", "ANGRY", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/emotion/disgusted", "DISGUSTED", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/emotion/happy", "HAPPY", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/emotion/surprised", "SURPRISED", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/emotion/fearful", "FEARFUL", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/emotion/sad", "SAD", renderer, *fontLarge, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/feedback/wow", "Let's Play", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/feedback/good", "Good Job", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/feedback/basic", "Not so Bad", renderer, *fontLarge, colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/feedback/bad", "Try Again", renderer, *fontLarge, colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/feedback/fail", "Epic Fail", renderer, *fontLarge, colorRed);

    textureCache.load<TTFFontTextureLoader>("str/info/thanks", "Thank YOU", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/playing", "For playing", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/welcome", "WELCOME", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/facesmash", "FACE SMASH", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/supporter", "SUPPORTER", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/wearegamee", "We are GAMEE", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/info/authors", "skypjack morbo84", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/info/support", "Support US", renderer, *fontNormal, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/buy", "Remove ads, unlock games", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/offer", "Offer US a coffee", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/info/oslicenses", "Open Source Licenses", renderer, *fontTiny, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/multiplayer/quick", "QUICK", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/multiplayer/invite", "INVITE", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/multiplayer/show", "SHOW", renderer, *fontTiny, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/audio", "AUDIO", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/video", "VIDEO", renderer, *fontTiny, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/haptic", "RUMBLE", renderer, *fontTiny, colorWhite);

    textureCache.load<TTFFontTextureLoader>("str/miss/10", "10", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/20", "20", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/50", "50", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/miss/100", "100", renderer, *fontSmall, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/smash/all/5", "5", renderer, *fontSmall, colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/smash/all/10", "10", renderer, *fontSmall, colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/smash/all/20", "20", renderer, *fontSmall, colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/smash/all/25", "25", renderer, *fontSmall, colorYellow);
    textureCache.load<TTFFontTextureLoader>("str/smash/default/50", "50", renderer, *fontSmall, colorBlue);
    textureCache.load<TTFFontTextureLoader>("str/smash/default/100", "100", renderer, *fontSmall, colorBlue);
    textureCache.load<TTFFontTextureLoader>("str/smash/default/200", "200", renderer, *fontSmall, colorBlue);
    textureCache.load<TTFFontTextureLoader>("str/smash/default/250", "250", renderer, *fontSmall, colorBlue);
    textureCache.load<TTFFontTextureLoader>("str/smash/rich/500", "500", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/rich/1000", "1000", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/rich/2000", "2000", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/rich/2500", "2500", renderer, *fontSmall, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/smash/bonus/x10", "x10", renderer, *fontSmall, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/reward/x2", "2x COMBO", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x3", "3x COMBO", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x4", "4x COMBO", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/x5", "5x COMBO", renderer, *fontLarge, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/reward/perfect", "PERFECT", renderer, *fontLarge, colorGreen);

    textureCache.load<TTFFontTextureLoader>("str/tutorial/face", "USE YOUR FACE", renderer, *fontNormal, colorRed);
    textureCache.load<TTFFontTextureLoader>("str/tutorial/touch", "TOUCH THESE", renderer, *fontNormal, colorGreen);
    textureCache.load<TTFFontTextureLoader>("str/tutorial/touchme", "TOUCH ME", renderer, *fontSmall, colorRed);

    textureCache.load<TTFFontTextureLoader>("str/menu/multiplayer", "1vs1", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/menu/play", "play", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/menu/training", "train", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/menu/achievements", "goal", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/menu/info", "about", renderer, *fontSmall, colorWhite);
    textureCache.load<TTFFontTextureLoader>("str/menu/leaderboard", "board", renderer, *fontSmall, colorWhite);
}


void prepareDebugStrings(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();
    auto fontTiny = Locator::TTFFontCache::ref().handle("font/debug/tiny");

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


void prepareCameraFrame(GameRenderer &renderer) {
    const auto &cameraService = Locator::Camera::ref();
    Locator::TextureCache::ref().reload<SDLTextureLoader>(
                "camera/frame", renderer, internalFormat, SDL_TEXTUREACCESS_STREAMING,
                cameraService.width(), cameraService.height());
}


void prepareOpponentFrame(GameRenderer &renderer) {
    Locator::TextureCache::ref().reload<SDLTextureLoader>(
                "opponent/frame", renderer, internalFormat, SDL_TEXTUREACCESS_STREAMING,
                opponentFrameWidth, opponentFrameHeight);
}


void prepareGameStuff(GameRenderer &renderer) {
    auto &textureCache = Locator::TextureCache::ref();
    textureCache.load<SDLTextureLoader>("target/logical", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
    textureCache.load<SDLTextureLoader>("target/recording", renderer, internalFormat, SDL_TEXTUREACCESS_TARGET, recordingWidth, recordingHeight);
}


}
