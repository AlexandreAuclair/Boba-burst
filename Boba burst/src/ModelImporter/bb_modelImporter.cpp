#include "bb_modelImporter.h"

// Include bibliothèque assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Affichage cout / cerr
#include <iostream>

#include "../Logger/Logger.h"


bool ModelImporter::LoadModel(const std::string& filepath, Mesh& mesh){
    
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        filepath, 
        aiProcess_Triangulate |                 // convertit les polygones en triangles
        aiProcess_GenSmoothNormals |            // génère normales lissees s'il y a un pb
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices         // fusionner sommets identiques
    );

    if (scene == nullptr){
        Logger::Err("Erreur import FBX : "
            + std::string(importer.GetErrorString()));
        return false;
    }

    mesh.nom = filepath;

    // Parcours du mesh
    for(unsigned int i = 0; i < scene->mNumMeshes; ++i){
        

        const aiMesh* meshData = scene->mMeshes[i]; 

        // Extraire les sommets
        for(unsigned int j = 0; j < meshData->mNumVertices; ++j){
            

            Sommet s{};
            
            // Position
            s.position = {
                meshData->mVertices[j].x,
                meshData->mVertices[j].y,
                meshData->mVertices[j].z
            };

            // Normale
            if (meshData->HasNormals())
            {
                s.normal = {
                    meshData->mNormals[j].x,
                    meshData->mNormals[j].y,
                    meshData->mNormals[j].z
                };
            }

            // Coordonnées UV
            if(meshData->HasTextureCoords(0)){
                s.UVcoord = {
                meshData->mTextureCoords[0][j].x,
                meshData->mTextureCoords[0][j].y
                };
            }
           mesh.sommets.push_back(s);

        }

        //Extraction des indexes
        for (unsigned int k = 0; k < meshData->mNumFaces; ++k){
            

            // Récupérer une face du maillage
            const aiFace& face = meshData->mFaces[k];

            // Parcourir la face pour récupérer les indices des sommets la composant
            for (unsigned int l = 0; l< face.mNumIndices; ++l){
                mesh.indexes.push_back(face.mIndices[l]);
            }
        }

    }

    Logger::Log("Importation reussie : " + filepath);

    return true;

}