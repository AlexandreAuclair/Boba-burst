#pragma once 

#include "../DataStruct/bb_mesh.h"

#include <string>
#include <memory>

class ModelImporter{
    public : 

    static bool LoadModel(const std::string& filepath, Mesh& mesh);

    static BoundingSphere CalculateBoundingSphere(const std::vector<Sommet>& sommets);
};