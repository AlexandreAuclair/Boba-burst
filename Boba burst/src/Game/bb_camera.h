#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "../DataStruct/bb_cameraConfig.h"
#include "../DataStruct/bb_frustumCulling.h"

enum class CameraMode
{
    FirstPerson,  // devant l'objet
    ThirdPerson   // derrière l'objet
};

class Camera {
public:
    explicit Camera(const CameraConfig& config = CameraConfig{});
    Camera(const Camera& cam)
        : m_config(cam.m_config)
        , m_mode(cam.m_mode)
        , m_position(cam.m_position)
        , m_target(cam.m_target)
        , m_up(cam.m_up)
        , m_initialized(cam.m_initialized) { }

    void ToggleMode();
    void SetMode(CameraMode mode);
    CameraMode GetMode() const { return m_mode; }

    /// @brief Met à jour la position et l'orientation selon le transform et l'orientation du joueur
    /// @param targetPos Position monde du joueur (ou point d'ancrage)
    /// @param yawDegrees Rotation horizontale en degrés (autour de Y)
    /// @param pitchDegrees Rotation verticale en degrés (autour de X)
    /// @param dt Delta time en secondes
    void Update(const glm::vec3& targetPos, float dt);
    void ProcessMouseMovement(float dx, float dy);

    /// @brief Calcule la matrice de vue (World -> View)
    glm::mat4 GetViewMatrix() const;

    /// @brief Calcule la matrice de projection (View -> Clip)
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    const glm::vec3& GetPosition() const { return m_position; }
    const glm::vec3& GetTarget() const { return m_target; }
    Frustum frustum;

private:
    glm::vec3 ComputeDesiredPosition(const glm::vec3& targetPos, float yawDegrees, float pitchDegrees) const;
    glm::vec3 ComputeDesiredTarget(const glm::vec3& targetPos, float yawDegrees, float pitchDegrees) const;

    CameraConfig m_config;
    CameraMode m_mode = CameraMode::FirstPerson;

    glm::vec3 m_position = glm::vec3(0.0f, 1.6f, 0.0f);
    glm::vec3 m_target   = glm::vec3(0.0f, 1.6f, -1.0f);
    glm::vec3 m_up       = glm::vec3(0.0f, 1.0f, 0.0f);
    bool m_initialized   = false;
    float m_yaw = 0.0f;
    float m_pitch = -90.0f;

};
