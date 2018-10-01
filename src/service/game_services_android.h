#ifndef FACE_SMASH_SERVICE_GAME_SERVICES_ANDROID_H
#define FACE_SMASH_SERVICE_GAME_SERVICES_ANDROID_H


#include "game_services.h"
#include <memory>

#ifdef __ANDROID__
#include <gpg/game_services.h>
#include <gpg/real_time_room.h>
#include <gpg/real_time_room_config_builder.h>
#include <gpg/i_real_time_event_listener.h>
#include <gpg/multiplayer_invitation.h>
#include <mutex>
#include "../common/constants.h"


class AMediaCodec;
class AMediaFormat;
#endif


namespace gamee {


struct GameServicesAndroid
        : GameServicesService, AchievementsService, LeaderboardsService, MultiplayerService
        #ifdef __ANDROID__
        , gpg::IRealTimeEventListener
        #endif
{
    GameServicesAndroid() noexcept;
    GameServicesAndroid& operator=(const GameServicesAndroid&) = default;
    GameServicesAndroid& operator=(GameServicesAndroid&&) = default;
    ~GameServicesAndroid() noexcept override;

    // GameServicesService interface
    Status status() const noexcept override;
    void signIn() noexcept override;
    void signOut() noexcept override;
    bool isSignedIn() const noexcept override;

    AchievementsService& achievements() noexcept override;
    LeaderboardsService& leaderboards() noexcept override;
    MultiplayerService& multiplayer() noexcept override;

    // AchievementsService interface
    void increment(FaceSmashAchievement, uint32_t) noexcept override;
    void unlock(FaceSmashAchievement) noexcept override;
    void showAllUI() noexcept override;
    void query(FaceSmashAchievement, std::function<void(bool)>) override;

    // LeaderboardsService interface
    void submitScore(FaceSmashLeaderboard, uint64_t) noexcept override;
    void showAllLeaderboardsUI() noexcept override;

    // MultiplayerService interface
    void quickMatch() noexcept override;
    void inviteFriend() noexcept override;
    void invitationInbox() noexcept override;
    void terminateMatch() noexcept override;
    void sendCameraFrame(const void *, int) override;
    void getOpponentFrame(std::function<void(const void *, int)>) noexcept override;
    void getOpponentScore(std::function<void(int)>) const noexcept override;
    void sendPlayerScore(int) override;
    bool readyPlayerOne() const noexcept override;
    void fetchInvitations() noexcept override;
    void handleInvitations() noexcept override;

#ifdef __ANDROID__
    void updateRoom(const gpg::RealTimeRoom &);
    void acceptInvitation(gpg::MultiplayerInvitation);
    void createRealTimeRoom(const gpg::RealTimeRoomConfig &);
    void showWaitingRoom();

    // gpg::IRealTimeEventListener
    void OnConnectedSetChanged(const gpg::RealTimeRoom &) override;
    void OnDataReceived(const gpg::RealTimeRoom &, const gpg::MultiplayerParticipant &, std::vector<std::uint8_t>, bool) override;
    void OnP2PConnected(const gpg::RealTimeRoom &, const gpg::MultiplayerParticipant &) override;
    void OnP2PDisconnected(const gpg::RealTimeRoom &, const gpg::MultiplayerParticipant &) override;
    void OnParticipantStatusChanged(const gpg::RealTimeRoom &, const gpg::MultiplayerParticipant &) override;
    void OnRoomStatusChanged(const gpg::RealTimeRoom &) override;

private:
    static constexpr auto frameSize = opponentFrameWidth * opponentFrameHeight * SDL_BITSPERPIXEL(internalFormat) / 8;
    static constexpr auto weKnowTheTimeIsNotExactlyThisOneButWeWantToSeeTheMultiplayerOnlineSoShutUp = 1000000/30;
    static constexpr auto sorryIMustLeaveAfterThisInterval = 3000;
    static constexpr auto frameRate = 30;
    static constexpr auto rawFormat = SDL_PIXELFORMAT_NV12;
    static constexpr auto videoFormat = "video/avc";
    static constexpr auto partSize = 1024;
    static constexpr auto count = frameSize / partSize;
    static constexpr auto numPlayers = 1;

    static AMediaFormat* makeEncoderMediaFormat();
    static AMediaFormat* makeDecoderMediaFormat();

    private:
    void feedEncoder(const void* input, size_t timeUs);
    // NOTE: pollEncoder may or may not call the passed function
    void pollEncoder(std::function<void(const void*, size_t len)>);
    void feedDecoder(const void* input, size_t len, size_t timeUs);
    void pollDecoder(void* output);
    void resetEncoder();
    void resetDecoder();

    std::unique_ptr<gpg::GameServices> gservice;
    std::unique_ptr<unsigned char[]> remoteFrameBuffer;
    std::unique_ptr<unsigned char[]> localFrameBuffer;
    bool hasPendingInvitations;
    uint32_t remoteFrameCounter;
    int partAccumulator;
    int sizeAccumulator;
    int opponentScore;
    Status sstatus;
    uint32_t frameProcessed;
    uint32_t frameCounter;
    bool isLeavingRoom;
    bool resultReceived;
    bool resultSent;
    gpg::RealTimeRoom room;
    mutable std::mutex mutex;
    AMediaFormat* encMediaFormat;
    AMediaCodec* encoder;
    AMediaFormat* decMediaFormat;
    AMediaCodec* decoder;
#endif
};


}


#endif // FACE_SMASH_SERVICE_GAME_SERVICES_ANDROID_H
