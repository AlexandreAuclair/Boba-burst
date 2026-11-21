#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <vulkan/vulkan.hpp>

#include "bb_sommet.h"
#include "bb_texture.h"
#include "../Utilities/bb_frustum.h"

struct Mesh
{
    std::string nom;
    glm::vec2 dimension; // pour boite englobantes
    std::vector<Sommet> sommets;
    std::vector<uint32_t> indexes;

    // Bounding Box locale
    AABB localBounds;

    // Vulkan Vertex Buffer
    vk::UniqueBuffer vertexBuffer;
    vk::UniqueDeviceMemory vertexBufferMemory;

    // Vulkan Index Buffer
    vk::UniqueBuffer indexBuffer;
    vk::UniqueDeviceMemory indexBufferMemory;

    void CalculateBounds()
    {
        if (sommets.empty())
            return;

        localBounds.min = glm::vec3(
            std::numeric_limits<float>::max());

        localBounds.max = glm::vec3(
            std::numeric_limits<float>::lowest());

        for (const auto& vertex : sommets)
        {
            localBounds.min = glm::min(
                localBounds.min, vertex.position);

            localBounds.max = glm::max(
                localBounds.max, vertex.position);
        }
    }
};