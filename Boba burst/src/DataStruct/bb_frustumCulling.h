#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

struct BoundingSphere
{
    glm::vec3 center;
    float radius;
};