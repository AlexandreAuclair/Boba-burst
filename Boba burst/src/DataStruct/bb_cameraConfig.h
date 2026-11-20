#pragma once 

#include <glm/glm.hpp>

struct CameraConfig {
    float fovDegrees = 45.0f; // Correspond à glm::pi<float>() /4
    float nearPlane  = 0.1f;
    float farPlane   = 1000.0f;

    // Décalage en 1re personne (ex. hauteur des yeux par rapport à l'origine du Player)
    glm::vec3 firstPersonOffset = glm::vec3(0.0f, 1.6f, 0.0f);

    // Décalage en 3e personne (hauteur + distance derrière la cible)
    glm::vec3 thirdPersonOffset = glm::vec3(0.0f, 2.5f, -6.0f);
    glm::vec3 thirdPersonLookOffset = glm::vec3(0.0f, 1.2f, 0.0f);

    // Lissage optionnel (0 = instantané, > 0 = interpolation fluide)
    float smoothSpeed = 12.0f;
};
