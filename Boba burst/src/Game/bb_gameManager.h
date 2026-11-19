#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "PhysicsWorld.h"


#include "../Actor/Character/bb_player.h"
#include "../Actor/Character/bb_bot.h"
#include "../Actor/Object/bb_trackObject.h"
#include "../Actor/Object/bb_bonusObject.h"
#include "../Actor/Object/bb_track.h"
#include "../Actor/Object/obstacle.h"

class GameManager
{
    GameManager();
    ~GameManager();

public:
    static GameManager &get()
    {
        static GameManager singleton;
        return singleton;
    }

    void update();

    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Bot>> bots;
    std::vector<std::unique_ptr<Track>> tracks;
    std::vector<std::unique_ptr<TrackObject>> trackObjects;
    std::vector<std::unique_ptr<BonusObject>> bonusObjects;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Character>> characters;
    std::unordered_map<int, Actor *> actorsById;

    
};