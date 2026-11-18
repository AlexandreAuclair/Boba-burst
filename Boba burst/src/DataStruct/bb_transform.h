#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

struct Transform{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale    = glm::vec3(1.0f);

    Transform() = default;

    // Construct directly from the "transform" JSON object
    Transform(const nlohmann::json& j) {
        if (j.contains("position")) {
            position = glm::vec3(j["position"][0], j["position"][1], j["position"][2]);
        }
        if (j.contains("rotation")) {
            rotation = glm::vec3(j["rotation"][0], j["rotation"][1], j["rotation"][2]);
        }
        if (j.contains("scale")) {
            scale = glm::vec3(j["scale"][0], j["scale"][1], j["scale"][2]);
        }
    }
};