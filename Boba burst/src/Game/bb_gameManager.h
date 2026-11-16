#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "../Actor/Character/bb_player.h"
#include "../Actor/Character/bb_bot.h"
#include "../Actor/Object/bb_trackObject.h"
#include "../Actor/Object/bb_bonusObject.h"
#include "../Actor/Object/bb_track.h"
#include "../DataStruct/bb_texture.h"

class GameManager {
    GameManager();
    ~GameManager();
public:
    static GameManager& get(){
        static GameManager singleton;
        return singleton;
    }

    std::shared_ptr<Mesh> GetModel(const std::string& modelName) {
        auto it = models.find(modelName);
        if (it != models.end()) {
            return it->second;
        }
        return nullptr;
    }
    std::shared_ptr<Texture> GetTexture(const std::string& textureName) {
        auto it = textures.find(textureName);
        if (it != textures.end()) {
            return it->second;
        }
        return nullptr;
    }

    void update();


    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<Bot>> bots;
    std::vector<std::unique_ptr<Track>> tracks;
    std::vector<std::unique_ptr<TrackObject>> trackObjects;
    std::vector<std::unique_ptr<BonusObject>> bonusObjects;
    std::vector<std::unique_ptr<Character>> characters;

    std::unordered_map<std::string, std::shared_ptr<Mesh>> models;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};