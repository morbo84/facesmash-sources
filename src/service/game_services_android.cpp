#include "game_services_android.h"


#ifdef __ANDROID__
#include "../locator/locator.hpp"
#include <gpg/achievement_manager.h>
#include <gpg/leaderboard_manager.h>
#include <gpg/platform_configuration.h>
#include <gpg/builder.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkMediaFormat.h>
#include <cstring>
#ifdef DEBUG
#include <gpg/debug.h>
#include <SDL_video.h>
#include <SDL_log.h>
#endif
#include <SDL_system.h>
#include <cassert>
#include <memory>
#include <chrono>


namespace gamee {
    static constexpr const char* achievementCode(FaceSmashAchievement a) {
    switch(a) {
        case FaceSmashAchievement::SMASH_ME_BABY:
            return "CgkI5IbdrqUREAIQAA";
        case FaceSmashAchievement::LITTLE_SMASHER:
            return "CgkI5IbdrqUREAIQAg";
        case FaceSmashAchievement::THE_SNIPER:
            return "CgkI5IbdrqUREAIQAw";
        case FaceSmashAchievement::GOD_SMASHER:
            return "CgkI5IbdrqUREAIQBA";
        case FaceSmashAchievement::MY_FIRST_COMBO:
            return "CgkI5IbdrqUREAIQBQ";
        case FaceSmashAchievement::COMBO_COLLECTOR_2X:
            return "CgkI5IbdrqUREAIQBg";
        case FaceSmashAchievement::COMBO_COLLECTOR_3X:
            return "CgkI5IbdrqUREAIQBw";
        case FaceSmashAchievement::COMBO_COLLECTOR_4X:
            return "CgkI5IbdrqUREAIQCA";
        case FaceSmashAchievement::COMBO_COLLECTOR_5X:
            return "CgkI5IbdrqUREAIQCQ";
        case FaceSmashAchievement::SMASH_THE_ANGRY:
            return "CgkI5IbdrqUREAIQCg";
        case FaceSmashAchievement::SMASH_THE_DISGUSTED:
            return "CgkI5IbdrqUREAIQCw";
        case FaceSmashAchievement::SMASH_THE_FEARFUL:
            return "CgkI5IbdrqUREAIQDA";
        case FaceSmashAchievement::SMASH_THE_HAPPY:
            return "CgkI5IbdrqUREAIQDQ";
        case FaceSmashAchievement::SMASH_THE_SAD:
            return "CgkI5IbdrqUREAIQDg";
        case FaceSmashAchievement::SMASH_THE_SURPRISED:
            return "CgkI5IbdrqUREAIQDw";
        case FaceSmashAchievement::IM_SO_HAPPY:
            return "CgkI5IbdrqUREAIQEA";
        case FaceSmashAchievement::BLUE_SMASH:
            return "CgkI5IbdrqUREAIQEQ";
        case FaceSmashAchievement::NO_PAIN_NO_GAME:
            return "CgkI5IbdrqUREAIQEg";
        case FaceSmashAchievement::KINDERGARTEN:
            return "CgkI5IbdrqUREAIQEw";
        case FaceSmashAchievement::READY_TO_SMASH:
            return "CgkI5IbdrqUREAIQFA";
        case FaceSmashAchievement::SMASH_IS_MY_JOB:
            return "CgkI5IbdrqUREAIQFQ";
        case FaceSmashAchievement::OH_MY_SMASH:
            return "CgkI5IbdrqUREAIQFg";
        case FaceSmashAchievement::SMASH_ME_CRY:
            return "CgkI5IbdrqUREAIQFw";
        case FaceSmashAchievement::FACE_SMASH_SUPPORTER:
            return "CgkI5IbdrqUREAIQHQ";
    }
}


static constexpr const char* leaderboardCode(FaceSmashLeaderboard l) {
    switch(l) {
        case FaceSmashLeaderboard::SCORE:
            return "CgkI5IbdrqUREAIQGA";
        case FaceSmashLeaderboard::FACES:
            return "CgkI5IbdrqUREAIQGQ";
    }
}


GameServicesAndroid::GameServicesAndroid() noexcept
    : remoteFrameBuffer{std::make_unique<unsigned char[]>(frameSize)},
      localFrameBuffer{std::make_unique<unsigned char[]>(frameSize)},
      hasPendingInvitations{false},
      remoteFrameCounter{},
      partAccumulator{},
      sizeAccumulator{},
      opponentScore{},
      sstatus{Status::SIGNED_OUT},
      frameProcessed{0},
      frameCounter{0},
      isLeavingRoom{false},
      mutex{},
      encMediaFormat{makeEncoderMediaFormat()},
      encoder{AMediaCodec_createEncoderByType(videoFormat)},
      decMediaFormat{makeDecoderMediaFormat()},
      decoder{AMediaCodec_createDecoderByType(videoFormat)}
{
    JNIEnv *env = (JNIEnv *) SDL_AndroidGetJNIEnv();
    jobject activity = (jobject) SDL_AndroidGetActivity();
    gpg::PlatformConfiguration conf;
    conf.SetActivity(activity);
    env->DeleteLocalRef(activity);

    gservice = gpg::GameServices::Builder{}.SetOnAuthActionFinished([this](gpg::AuthOperation op, gpg::AuthStatus status) {
        std::lock_guard guard{mutex};

        switch (op) {
        case gpg::AuthOperation::SIGN_IN:
            switch (status) {
            case gpg::AuthStatus::VALID:
                sstatus = Status::SIGNED_IN;
                break;
            default:
                sstatus = Status::SIGNED_OUT;
                break;
            }
            break;
        case gpg::AuthOperation::SIGN_OUT:
            sstatus = Status::SIGNED_OUT;
            break;
        }

        (void)guard;
    }).SetOnMultiplayerInvitationEvent([this](gpg::MultiplayerEvent, std::string, gpg::MultiplayerInvitation) {
        hasPendingInvitations = true;
#ifdef DEBUG
    }).SetOnLog([](gpg::LogLevel lvl, const std::string &msg) {
        SDL_Log("GPG Log: %s", msg.c_str());
#endif
    }).Create(conf);

    sstatus = Status::SIGNING_IN;

    // configure codecs stuff
    AMediaCodec_configure(encoder, encMediaFormat, nullptr, nullptr, AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    AMediaCodec_start(encoder);
    AMediaCodec_configure(decoder, decMediaFormat, nullptr, nullptr, 0);
    AMediaCodec_start(decoder);
}

GameServicesAndroid::~GameServicesAndroid() noexcept {
    AMediaCodec_delete(encoder);
    AMediaFormat_delete(encMediaFormat);
    AMediaCodec_delete(decoder);
    AMediaFormat_delete(decMediaFormat);
}


GameServicesService::Status GameServicesAndroid::status() const noexcept {
    std::lock_guard guard{mutex};
    return sstatus;
    (void)guard;
}


void GameServicesAndroid::signIn() noexcept {
    gservice->StartAuthorizationUI();
}


void GameServicesAndroid::signOut() noexcept {
    {
        std::lock_guard guard{mutex};
        sstatus = Status::SIGNING_OUT;
        (void)guard;
    }

    gservice->SignOut();
}


void GameServicesAndroid::increment(FaceSmashAchievement a, uint32_t steps) noexcept {
    gservice->Achievements().Increment(achievementCode(a), steps);
}


void GameServicesAndroid::unlock(FaceSmashAchievement a) noexcept {
    gservice->Achievements().Unlock(achievementCode(a));
}


void GameServicesAndroid::showAllUI() noexcept {
    gservice->Achievements().ShowAllUIBlocking(std::chrono::seconds{3});
}


void GameServicesAndroid::query(FaceSmashAchievement a, std::function<void(bool)> func) {
    gservice->Achievements().Fetch(achievementCode(a), [func = std::move(func)](const gpg::AchievementManager::FetchResponse &res) {
        func((res.status == gpg::ResponseStatus::VALID || res.status == gpg::ResponseStatus::VALID_BUT_STALE) && (res.data.State() == gpg::AchievementState::UNLOCKED));
    });
}


bool GameServicesAndroid::isSignedIn() const noexcept {
    return gservice->IsAuthorized();
}


AchievementsService& GameServicesAndroid::achievements() noexcept {
    return *this;
}


LeaderboardsService& GameServicesAndroid::leaderboards() noexcept {
    return *this;
}


MultiplayerService& GameServicesAndroid::multiplayer() noexcept {
    return *this;
}


void GameServicesAndroid::submitScore(FaceSmashLeaderboard l, uint64_t score) noexcept {
    gservice->Leaderboards().SubmitScore(leaderboardCode(l), score);
}


void GameServicesAndroid::showAllLeaderboardsUI() noexcept {
    gservice->Leaderboards().ShowAllUIBlocking(std::chrono::seconds{3});
}


void GameServicesAndroid::quickMatch() noexcept {
    if(!isLeavingRoom) {
        createRealTimeRoom(gpg::RealTimeRoomConfig::Builder()
                                   .SetMinimumAutomatchingPlayers(numPlayers)
                                   .SetMaximumAutomatchingPlayers(numPlayers)
                                   .Create());
    }
}


void GameServicesAndroid::inviteFriend() noexcept {
    if(!isLeavingRoom) {
        gservice->RealTimeMultiplayer().ShowPlayerSelectUI(numPlayers, numPlayers, true,
                                                           [this](const gpg::RealTimeMultiplayerManager::PlayerSelectUIResponse &res) {
                                                               createRealTimeRoom(
                                                                       gpg::RealTimeRoomConfig::Builder().PopulateFromPlayerSelectUIResponse(
                                                                               res).Create());
                                                           });
    }
}


void GameServicesAndroid::invitationInbox() noexcept {
    if(!isLeavingRoom) {
        gservice->RealTimeMultiplayer().ShowRoomInboxUI(
                [this](const gpg::RealTimeMultiplayerManager::RoomInboxUIResponse res) {
                    if (gpg::IsSuccess(res.status)) {
                        acceptInvitation(res.invitation);
#ifdef DEBUG
                    } else {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Challenges [Invitation Inbox]",
                                                 gpg::DebugString(res.status).c_str(), nullptr);
#endif
                    }
                });
    }
}


void GameServicesAndroid::terminateMatch() noexcept {
    if(room.Valid()) {
        gservice->RealTimeMultiplayer().LeaveRoom(room, [this](const gpg::ResponseStatus &response) {
            if(gpg::IsError(response)) {
                // ... uhm ... try again? not sure what to do here actually :-D
                terminateMatch();
            } else if(gpg::IsSuccess(response)) {
                isLeavingRoom = false;
                updateRoom({});
            }
        });

        isLeavingRoom = true;
    } else {
        isLeavingRoom = false;
    }
}


void GameServicesAndroid::getOpponentFrame(std::function<void(const void *, int)> func) noexcept {
    std::lock_guard guard{mutex};
    auto *ptr = localFrameBuffer.get();
    pollDecoder(ptr);
    func(ptr, frameSize);
    (void)guard;
}


void GameServicesAndroid::getOpponentScore(std::function<void(int)> func) const noexcept {
    std::lock_guard guard{mutex};
    func(opponentScore);
    (void)guard;
}


void GameServicesAndroid::sendCameraFrame(const void *pixels, [[maybe_unused]] int size) {
    assert(size == frameSize);

    if(room.Valid()) {
        feedEncoder(pixels, frameProcessed++ * weKnowTheTimeIsNotExactlyThisOneButWeWantToSeeTheMultiplayerOnlineSoShutUp);

        pollEncoder([this](const void *pixels, std::size_t size) {
            const auto *frame = static_cast<const std::uint8_t *>(pixels);

            auto currCounter = frameCounter++;
            std::vector<std::uint8_t> data;

            std::uint8_t count = size / partSize + 1;

            for(std::uint8_t i = {}; i < count; ++i) {
                const auto first = frame + (partSize*i);
                const auto last = std::min(first + partSize, frame + size);
                std::uint16_t length = last - first;

                data.clear();

                data.push_back('F');
                data.push_back(static_cast<std::uint8_t>((currCounter & 0x000000FF) >> 0));
                data.push_back(static_cast<std::uint8_t>((currCounter & 0x0000FF00) >> 8));
                data.push_back(i);
                data.push_back(count);
                data.push_back(static_cast<std::uint8_t>((length & 0x000000FF) >> 0));
                data.push_back(static_cast<std::uint8_t>((length & 0x0000FF00) >> 8));
                data.insert(data.end(), first, last);

                gservice->RealTimeMultiplayer().SendUnreliableMessageToOthers(room, data);
            }
        });
    }
}


void GameServicesAndroid::sendPlayerScore(int score) {
    if(room.Valid()) {
        std::uint32_t score32(score);
        std::vector<std::uint8_t> data;

        data.push_back('S');
        data.push_back(static_cast<std::uint8_t>((score32 & 0x000000FF) >> 0));
        data.push_back(static_cast<std::uint8_t>((score32 & 0x0000FF00) >> 8));
        data.push_back(static_cast<std::uint8_t>((score32 & 0x00FF0000) >> 16));
        data.push_back(static_cast<std::uint8_t>((score32 & 0xFF000000) >> 24));

        gservice->RealTimeMultiplayer().SendUnreliableMessageToOthers(room, data);
    }
}


bool GameServicesAndroid::readyPlayerOne() const noexcept {
    return !isLeavingRoom && room.Valid() && room.Status() == gpg::RealTimeRoomStatus::ACTIVE;
}


void GameServicesAndroid::fetchInvitations() noexcept {
    gservice->RealTimeMultiplayer().FetchInvitations([this](const gpg::RealTimeMultiplayerManager::FetchInvitationsResponse &response) {
        hasPendingInvitations = gpg::IsSuccess(response.status) && !response.invitations.empty();
    });
}


void GameServicesAndroid::handleInvitations() noexcept {
    if(hasPendingInvitations) {
        hasPendingInvitations = false;
        invitationInbox();
    }

}


void GameServicesAndroid::updateRoom(const gpg::RealTimeRoom &room) {
    std::lock_guard guard{mutex};
    this->room = room;
    (void)guard;
}


void GameServicesAndroid::acceptInvitation(gpg::MultiplayerInvitation invitation) {
    gpg::RealTimeMultiplayerManager::RealTimeRoomResponse result =
            gservice->RealTimeMultiplayer().AcceptInvitationBlocking(invitation, this);

    if(gpg::IsSuccess(result.status)) {
        updateRoom(result.room);
        showWaitingRoom();
#ifdef DEBUG
    } else {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Challenges [Accept Invitation]", gpg::DebugString(result.status).c_str(), nullptr);
#endif
    }
}


void GameServicesAndroid::createRealTimeRoom(const gpg::RealTimeRoomConfig &config) {
    gservice->RealTimeMultiplayer().CreateRealTimeRoom(config, this, [this](const gpg::RealTimeMultiplayerManager::RealTimeRoomResponse &res) {
        if(gpg::IsSuccess(res.status)) {
            updateRoom(res.room);
            showWaitingRoom();
#ifdef DEBUG
        } else {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Challenges [Create RealTime Room]", gpg::DebugString(res.status).c_str(), nullptr);
#endif
        }
    });
}


void GameServicesAndroid::showWaitingRoom() {
    gservice->RealTimeMultiplayer().ShowWaitingRoomUI(room, numPlayers, [this](const gpg::RealTimeMultiplayerManager::WaitingRoomUIResponse &res) {
        if(gpg::IsSuccess(res.status)) {
            // theoretically, next time someone invokes readyPlayerOne(), the room results active and thus the game starts
            // because of this, there is nothing to do here, updating the room was all what is needed so far... hopefully
            frameProcessed = {};
            frameCounter = {};
            isLeavingRoom = false;
            resultReceived = false;
            resultSent = false;
            remoteFrameCounter = {};
            partAccumulator = {};
            sizeAccumulator = {};
            resetEncoder();
            resetDecoder();
            updateRoom(res.room);
#ifdef DEBUG
        } else {
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Challenges [Show Waiting Room]", gpg::DebugString(res.status).c_str(), nullptr);
#endif
        }
    });
}


void GameServicesAndroid::OnConnectedSetChanged(const gpg::RealTimeRoom &room) {
    updateRoom(room);
}


void GameServicesAndroid::OnDataReceived(const gpg::RealTimeRoom &room, const gpg::MultiplayerParticipant &, std::vector<std::uint8_t> msg, bool) {
    updateRoom(room);

    const auto *raw = static_cast<const unsigned char *>(msg.data());
    const auto mtype = *raw;

    std::lock_guard guard{mutex};

    switch(mtype) {
    case 'F':
        [this](const unsigned char *raw) {
            std::uint16_t remoteCounter = (static_cast<std::uint16_t>(*(raw+1))<<0) | (static_cast<std::uint16_t>(*(raw+2))<<8);
            std::uint8_t part = static_cast<std::uint8_t>(*(raw+3));
            std::uint8_t count = static_cast<std::uint8_t>(*(raw+4));
            std::uint16_t length = (static_cast<std::uint16_t>(*(raw+5))<<0) | (static_cast<std::uint16_t>(*(raw+6))<<8);
            const unsigned char *pixels = raw+7;

            if(remoteCounter >= remoteFrameCounter) {
                if(remoteCounter > remoteFrameCounter) {
                    remoteFrameCounter = remoteCounter;
                    partAccumulator = {};
                    sizeAccumulator = {};
                };

                std::copy_n(pixels, length, remoteFrameBuffer.get() + part*partSize);
                sizeAccumulator += length;
                ++partAccumulator;

                if(partAccumulator == count) {
                    auto *ptr = remoteFrameBuffer.get();
                    feedDecoder(ptr, sizeAccumulator, remoteFrameCounter * weKnowTheTimeIsNotExactlyThisOneButWeWantToSeeTheMultiplayerOnlineSoShutUp);
                }
            }
        }(raw);
        break;
    case 'S':
        [this](const unsigned char *raw) {
            if(!resultReceived) {
                opponentScore = (static_cast<std::uint32_t>(*(raw + 1)) << 0)
                                | (static_cast<std::uint32_t>(*(raw + 2)) << 8)
                                | (static_cast<std::uint32_t>(*(raw + 3)) << 16)
                                | (static_cast<std::uint32_t>(*(raw + 4)) << 24);
            }
        }(raw);
        break;
    case 'R':
        [this](const unsigned char *raw) {
            opponentScore = (static_cast<std::uint32_t>(*(raw + 1)) << 0)
                            | (static_cast<std::uint32_t>(*(raw + 2)) << 8)
                            | (static_cast<std::uint32_t>(*(raw + 3)) << 16)
                            | (static_cast<std::uint32_t>(*(raw + 4)) << 24);

            resultReceived = true;

            if(resultSent) {
                terminateMatch();
            }
        }(raw);
        break;
    }

    (void)guard;
}


void GameServicesAndroid::OnP2PConnected(const gpg::RealTimeRoom &room, const gpg::MultiplayerParticipant &participant) {
    updateRoom(room);
}


void GameServicesAndroid::OnP2PDisconnected(const gpg::RealTimeRoom &room, const gpg::MultiplayerParticipant &participant) {
    updateRoom(room);
}


void GameServicesAndroid::OnParticipantStatusChanged(const gpg::RealTimeRoom &room, const gpg::MultiplayerParticipant &participant) {
    updateRoom(room);
}


void GameServicesAndroid::OnRoomStatusChanged(const gpg::RealTimeRoom &room) {
    updateRoom(room);
}


void GameServicesAndroid::feedEncoder(const void* input, size_t timeUs) {
    // NOTE: I'm assuming the resolution of the input buffer is opponentFrameWidth * opponentFrameHeight
    auto signedInputBufId = AMediaCodec_dequeueInputBuffer(encoder, sorryIMustLeaveAfterThisInterval);

    if (signedInputBufId >= 0) {
        auto inputBufferId = static_cast<size_t>(signedInputBufId);
        size_t size;
        auto* buf = AMediaCodec_getInputBuffer(encoder, inputBufferId, &size);
        SDL_ConvertPixels(opponentFrameWidth, opponentFrameHeight, internalFormat, input,
                          opponentFrameWidth * SDL_BYTESPERPIXEL(internalFormat), rawFormat,
                          buf, opponentFrameWidth * SDL_BYTESPERPIXEL(rawFormat));

        constexpr auto n = static_cast<size_t>(opponentFrameWidth * opponentFrameHeight) * 12U / 8U;
        AMediaCodec_queueInputBuffer(encoder, inputBufferId, 0, n, timeUs, 0);
    }
}


void GameServicesAndroid::pollEncoder(std::function<void(const void*, size_t len)> func) {
    AMediaCodecBufferInfo videoBufInfo;
    auto signedOutputBufId = AMediaCodec_dequeueOutputBuffer(encoder, &videoBufInfo, sorryIMustLeaveAfterThisInterval);
    if(signedOutputBufId >= 0) {
        auto outputBufferId = static_cast<size_t>(signedOutputBufId);
        if(videoBufInfo.size > 0) {
            size_t dummySize;
            const auto* buf = AMediaCodec_getOutputBuffer(encoder, outputBufferId, &dummySize);
            func(buf, static_cast<size_t>(videoBufInfo.size));
        }

        AMediaCodec_releaseOutputBuffer(encoder, outputBufferId, false);
    }
}


void GameServicesAndroid::feedDecoder(const void* input, size_t len, size_t timeUs) {
    auto signedInputBufId = AMediaCodec_dequeueInputBuffer(decoder, sorryIMustLeaveAfterThisInterval);
    if (signedInputBufId >= 0) {
        auto inputBufferId = static_cast<size_t>(signedInputBufId);
        size_t sizeDec;
        auto* buf = AMediaCodec_getInputBuffer(decoder, inputBufferId, &sizeDec);
        std::copy_n(static_cast<const char*>(input), len, buf);
        AMediaCodec_queueInputBuffer(decoder, inputBufferId, 0, len, timeUs, 0);
    }
}


void GameServicesAndroid::pollDecoder(void *output) {
    AMediaCodecBufferInfo videoBufInfo;
    auto signedOutputBufId = AMediaCodec_dequeueOutputBuffer(decoder, &videoBufInfo, 10000);
    if(signedOutputBufId >= 0) {
        auto outputDecBuffer = static_cast<size_t>(signedOutputBufId);
        size_t size;
        const auto* outDecBuf = AMediaCodec_getOutputBuffer(decoder, outputDecBuffer, &size);

        SDL_ConvertPixels(opponentFrameWidth, opponentFrameHeight, rawFormat, outDecBuf, opponentFrameWidth * SDL_BYTESPERPIXEL(rawFormat),
                          internalFormat, output, opponentFrameWidth * SDL_BYTESPERPIXEL(internalFormat));

        AMediaCodec_releaseOutputBuffer(decoder, outputDecBuffer, false);
    }
}


AMediaFormat *GameServicesAndroid::makeEncoderMediaFormat() {
    auto* mediaFormat = AMediaFormat_new();
    AMediaFormat_setString(mediaFormat, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_WIDTH, opponentFrameWidth);
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_HEIGHT, opponentFrameHeight);
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_BIT_RATE, 300000);
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_FRAME_RATE, frameRate);
    AMediaFormat_setInt32(mediaFormat, "capture-rate", frameRate);
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_I_FRAME_INTERVAL, 3);
    /**
     * NOTE: the 0x15 value (COLOR_FormatYUV420PackedSemiPlanar) is deprecated;
     * instead, we should use COLOR_FormatYUV420Flexible (0x7f420888), because the Media Codec
     * documentations states that "All video codecs support flexible YUV 4:2:0 buffers since LOLLIPOP_MR1".
     * But flexibility comes at a cost:  we cannot impose Android anymore what color format to
     * accept, so it will be free to pick a format from the YUV420 set.
     *
     * For now, we try to force 0x15 (NV12, actually) and see whether someone complains.
     * In the future, we might try to query the mediaFormat for the actual color format used
     * and use the corresponding SDL pixelformat.
     */
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_COLOR_FORMAT, 0x15);

    return mediaFormat;
}


AMediaFormat *GameServicesAndroid::makeDecoderMediaFormat() {
    auto* mediaFormat = AMediaFormat_new();
    AMediaFormat_setString(mediaFormat, AMEDIAFORMAT_KEY_MIME, "video/avc");
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_WIDTH, opponentFrameWidth);
    AMediaFormat_setInt32(mediaFormat, AMEDIAFORMAT_KEY_HEIGHT, opponentFrameHeight);
    AMediaFormat_setInt32(mediaFormat, "capture-rate", frameRate);

    return mediaFormat;
}


void GameServicesAndroid::resetEncoder() {
    AMediaCodec_flush(encoder);
    AMediaCodec_stop(encoder);
    // maybe here we'll have to configure the codec again
    AMediaCodec_configure(encoder, encMediaFormat, nullptr, nullptr, AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
    AMediaCodec_start(encoder);
}


void GameServicesAndroid::resetDecoder() {
    AMediaCodec_flush(decoder);
    AMediaCodec_stop(decoder);
    // maybe here we'll have to configure the codec again
    AMediaCodec_configure(decoder, decMediaFormat, nullptr, nullptr, 0);
    AMediaCodec_start(decoder);
}


}


#else


namespace gamee {


GameServicesAndroid::GameServicesAndroid() noexcept {}
GameServicesAndroid::~GameServicesAndroid() noexcept {}
GameServicesService::Status GameServicesAndroid::status() const noexcept {  return GameServicesService::Status::SIGNED_OUT; }
void GameServicesAndroid::signIn() noexcept {}
void GameServicesAndroid::signOut() noexcept {}
void GameServicesAndroid::unlock(FaceSmashAchievement) noexcept {}
void GameServicesAndroid::increment(FaceSmashAchievement, uint32_t) noexcept {}
void GameServicesAndroid::showAllUI() noexcept {}
void GameServicesAndroid::query(FaceSmashAchievement, std::function<void(bool)> func) { func(true); }
bool GameServicesAndroid::isSignedIn() const noexcept { return false; }
void GameServicesAndroid::submitScore(FaceSmashLeaderboard, uint64_t) noexcept {}
void GameServicesAndroid::showAllLeaderboardsUI() noexcept {}
void GameServicesAndroid::quickMatch() noexcept {}
void GameServicesAndroid::inviteFriend() noexcept {}
void GameServicesAndroid::invitationInbox() noexcept {}
void GameServicesAndroid::terminateMatch() noexcept {}
void GameServicesAndroid::getOpponentFrame(std::function<void(const void *, int)> func) noexcept { func(nullptr, 0); }
void GameServicesAndroid::getOpponentScore(std::function<void(int)> func) const noexcept { func(0); }
void GameServicesAndroid::sendCameraFrame(const void *, int) {}
void GameServicesAndroid::sendPlayerScore(int) {}
bool GameServicesAndroid::readyPlayerOne() const noexcept { return false; }
void GameServicesAndroid::fetchInvitations() noexcept {}
void GameServicesAndroid::handleInvitations() noexcept {}
AchievementsService& GameServicesAndroid::achievements() noexcept { return *this; }
LeaderboardsService& GameServicesAndroid::leaderboards() noexcept { return *this; }
MultiplayerService& GameServicesAndroid::multiplayer() noexcept { return *this; }


}

#endif
