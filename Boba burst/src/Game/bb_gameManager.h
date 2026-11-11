#pragma once

#include <vector>
#include <memory>

#include "Actor/Character/bb_player.h"
#include "Actor/Character/bb_bot.h"
#include "Actor/Object/bb_trackObject.h"
#include "Actor/Object/bb_track.h"

class GameManager {
    GameManager();
    ~GameManager();
public:
    static GameManager& get(){
        static GameManager singleton;
        return singleton;
    }

    void registerPlayer(Player* p);
    void registerBot(Bot* p);
    void registerTrack(Track* p);
    void registerTrackObject(TrackObject* p);

    void update();

private:
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Bot>> bots;
    std::vector<std::unique_ptr<Track>> tracks;
    std::vector<std::unique_ptr<TrackObject>> trackObjects;
};