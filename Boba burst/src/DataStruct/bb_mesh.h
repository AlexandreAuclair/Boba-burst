#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "bb_sommet.h"
#include "bb_texture.h"

struct Mesh{
    std::string nom;
    glm::vec2 dimension; // pour boite englobantes
    std::vector<Sommet> sommets;
    std::vector<uint32_t> indexes;

    // Vulkan Vertex Buffer
    vk::UniqueBuffer vertexBuffer;
    vk::UniqueDeviceMemory vertexBufferMemory;

    // Vulkan Index Buffer
    vk::UniqueBuffer indexBuffer;
    vk::UniqueDeviceMemory indexBufferMemory;
};