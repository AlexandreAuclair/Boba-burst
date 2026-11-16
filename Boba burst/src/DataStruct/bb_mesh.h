#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "bb_sommet.h"

struct Mesh{
    std::string nom;
    glm::vec2 dimension; // pour boite englobantes
    std::vector<Sommet> sommets;
    std::vector<uint32_t> indexes;
};