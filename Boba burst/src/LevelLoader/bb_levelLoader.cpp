#include "bb_levelLoader.h"

#include <fstream>


#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

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
            std::string src = modelJson.at("modelPath").get<std::string>();

            // On ne le charge que s'il n'est pas déjà dans le GameManager
            if (gm.models.find(nom) == gm.models.end())
            {
                Logger::Log("Loading model into cache : " + nom + " (" + src + ")");
                auto mesh = std::make_shared<Mesh>();
                if(!ModelImporter::LoadModel(src, *mesh)){
                    Logger::Err("Impossible de charger le model: " + src);
                }
                gm.models[nom] = mesh;
            }
        }

        for(const auto& textureJson : j.at("textures")){
            std::string nom = textureJson.at("textureName").get<std::string>();
            std::string src = textureJson.at("texturePath").get<std::string>();

            // On ne le charge que s'il n'est pas déjà dans le GameManager
            if (gm.textures.find(nom) == gm.textures.end())
            {
                Logger::Log("Loading texture into cache : " + nom + " (" + src + ")");
                auto tex = std::make_shared<Texture>();
                tex->pixels = stbi_load(src.c_str(), &tex->width, &tex->height, &tex->channels, STBI_rgb_alpha);
                if (!tex->pixels){
                    Logger::Err("Impossible de charger la texture : " + src);
                }
                gm.textures[nom] = tex;
            }
        }

        // Création des Actors et association avec leur modèle
        for (const auto& actorJson : j.at("actors"))
        {
            std::string type = actorJson.at("type").get<std::string>();
            std::string modelName = actorJson.at("modelName").get<std::string>();
            std::string textureName = actorJson.at("textureName").get<std::string>();
            int id = actorJson.at("id").get<int>();
            Transform transform = actorJson.contains("transform") ? Transform(actorJson["transform"]) : Transform();
            

            auto mesh = gm.GetModel(modelName);
            auto texture = gm.GetTexture(textureName);
            if (!mesh)
            {
                Logger::Err("Modèle introuvable dans le cache : " + modelName);
                continue;
            }

            if (type == "Player") {
                auto player = std::make_unique<Player>(id,transform);
                player->mesh = mesh;
                player->mesh->texture = texture;
                gm.players.push_back(std::move(player));
            }
            else if (type == "Track") {
                int nbSections = actorJson.at("nbSections").get<int>();
                auto track = std::make_unique<Track>(id,transform,nbSections);
                track->mesh = mesh;
                track->mesh->texture = texture;
                gm.tracks.push_back(std::move(track));
            }
            else if (type == "Character") {
                auto character = std::make_unique<Character>(id, transform);
                character->mesh = mesh;
                character->mesh->texture = texture;
                gm.characters.push_back(std::move(character));
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

void LevelLoader::FreeModelAndTextures(){
    auto& gm = GameManager::get();
    for(auto& [name, tex] : gm.textures){
        stbi_image_free(tex->pixels);
    }
}