#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "bb_sommet.h"

struct Mesh{
    Mesh(const std::string &name) : nom(name){}
    ~Mesh() = default;

    std::string GetName(){ return nom; }

private:
    std::string nom;
    glm::vec2 dimension; // pour boite englobantes
    std::vector<Sommet> sommets;
    std::vector<uint32_t> indices;
};