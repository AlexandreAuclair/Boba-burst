#include "bb_levelLoader.h"

#include <fstream>

#include "../Logger/Logger.h"
using nlohmann::json;

LevelLoader::LevelLoader() {

}

LevelLoader::~LevelLoader(){

}

bool LevelLoader::LoadLevel(int n, std::string path)
{
    json j;

    j["playerName"] = "HeroMax";
    j["level"] = 5;
    j["position"] = { 10.5, 20.0, 0.0 };

    // Écriture
    {
        std::ofstream fichier("config.json");

        if (!fichier.is_open())
        {
            Logger::Log("Impossible de creer config.json");
            return false;
        }

        fichier << j.dump(4);
    }

    // Lecture
    json j2;

    {
        std::ifstream fichierEntree("config.json");

        if (!fichierEntree.is_open())
        {
            Logger::Log("Impossible de lire config.json");
            return false;
        }

        try
        {
            fichierEntree >> j2;
        }
        catch (const json::parse_error& e)
        {
            Logger::Log(e.what());
            return false;
        }
    }

    // Récupération
    std::string nom = j2.at("playerName").get<std::string>();
    int niveau = j2.at("level").get<int>();

    Logger::Log("Joueur : " + nom + " (Niveau " + std::to_string(niveau) + ")");

    return true;
}