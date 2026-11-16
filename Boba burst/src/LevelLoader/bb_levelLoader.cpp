#include "bb_levelLoader.h"

#include <fstream>

#include "../Logger/Logger.h"
#include "../ModelImporter/bb_modelImporter.h"
#include "../Game/bb_gameManager.h"

using nlohmann::json;

LevelLoader::LevelLoader() {

}

LevelLoader::~LevelLoader(){

}

bool LevelLoader::LoadLevel(std::string path)
{
    // Lecture
    json j;

    
    std::ifstream fichierEntree(path);
    if (!fichierEntree.is_open())
    {
        Logger::Err("Impossible de lire le fichier : " + path);
        return false;
    }

    try
    {
        fichierEntree >> j;
    }
    catch (const json::parse_error& e)
    {
        Logger::Err(std::string("Erreur de parsing JSON : ") + e.what());
        return false;
    }

    try
    {
        auto& gm = GameManager::get();

        // Chargement de la liste des modèles (une seule fois chacun)
        for (const auto& modelJson : j.at("models"))
        {
            std::string nom = modelJson.at("modelName").get<std::string>();
            std::string src = modelJson.at("path").get<std::string>();

            // On ne le charge que s'il n'est pas déjà dans le GameManager
            if (gm.models.find(nom) == gm.models.end())
            {
                Logger::Log("Loading model into cache : " + nom + " (" + src + ")");
                auto mesh = std::make_shared<Mesh>();
                ModelImporter::LoadModel(src, *mesh); // Adapte '*' selon la signature de LoadModel
                gm.models[nom] = mesh;
            }
        }

        // Création des Actors et association avec leur modèle
        for (const auto& actorJson : j.at("actors"))
        {
            std::string type = actorJson.at("type").get<std::string>();
            std::string modelName = actorJson.at("modelName").get<std::string>();
            int id = actorJson.at("id").get<int>();
            Transform transform = actorJson.contains("transform") ? Transform(actorJson["transform"]) : Transform();
            

            auto mesh = gm.GetModel(modelName);
            if (!mesh)
            {
                Logger::Err("Modèle introuvable dans le cache : " + modelName);
                continue;
            }

            if (type == "Player")
            {

                auto player = std::make_unique<Player>(id,transform);
                player->mesh = mesh; // Ou passage via constructeur : std::make_shared<Player>(mesh)
                gm.players.push_back(std::move(player));
            }
            else if (type == "Track")
            {

                int nbSections = actorJson.at("nbSections").get<int>();
                auto track = std::make_unique<Track>(id,transform,nbSections);
                track->mesh = mesh;
                gm.tracks.push_back(std::move(track));
            }
            else
            {
                Logger::Warn("Type d'actor inconnu : " + type);
            }
        }
    }
    catch (const json::exception& e)
    {
        Logger::Err(std::string("Structure JSON invalide : ") + e.what());
        return false;
    }

    return true;
}