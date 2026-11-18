#pragma once

#include <glm/glm.hpp>

struct LightData {
    glm::vec4 direction;
    glm::vec4 color;
    glm::vec4 cameraPosition;
};