#include "bb_camera.h"
#include <algorithm>

Camera::Camera(const CameraConfig& config)
    : m_config(config) {}

void Camera::ToggleMode() {
    m_mode = (m_mode == CameraMode::FirstPerson)
        ? CameraMode::ThirdPerson
        : CameraMode::FirstPerson;
}

void Camera::SetMode(CameraMode mode) {
    m_mode = mode;
}

glm::vec3 Camera::ComputeDesiredPosition(const glm::vec3& targetPos, float yawDegrees, float pitchDegrees) const {
    float yawRad   = glm::radians(yawDegrees);
    float pitchRad = glm::radians(pitchDegrees);

    // Direction avant (convention main droite, -Z vers l'avant quand yaw = 0)
    glm::vec3 forward;
    forward.x = std::cos(pitchRad) * std::sin(yawRad);
    forward.y = std::sin(pitchRad);
    forward.z = -std::cos(pitchRad) * std::cos(yawRad);
    forward = glm::normalize(forward);

    if (m_mode == CameraMode::FirstPerson) {
        return targetPos + m_config.firstPersonOffset;
    }

    // 3e personne : recule derrière la direction horizontale + élévation
    glm::vec3 horizontalForward = glm::normalize(glm::vec3(std::sin(yawRad), 0.0f, -std::cos(yawRad)));
    glm::vec3 right = glm::normalize(glm::cross(horizontalForward, m_up));

    return targetPos
         - horizontalForward * std::abs(m_config.thirdPersonOffset.z)
         + m_up * m_config.thirdPersonOffset.y
         + right * m_config.thirdPersonOffset.x;
}

glm::vec3 Camera::ComputeDesiredTarget(const glm::vec3& targetPos, float yawDegrees, float pitchDegrees) const {
    float yawRad   = glm::radians(yawDegrees);
    float pitchRad = glm::radians(pitchDegrees);

    if (m_mode == CameraMode::FirstPerson) {
        glm::vec3 eyePos = targetPos + m_config.firstPersonOffset;
        glm::vec3 forward;
        forward.x = std::cos(pitchRad) * std::sin(yawRad);
        forward.y = std::sin(pitchRad);
        forward.z = -std::cos(pitchRad) * std::cos(yawRad);
        return eyePos + glm::normalize(forward);
    }

    return targetPos + m_config.thirdPersonLookOffset;
}

void Camera::ProcessMouseMovement(const glm::vec3& targetPos, float dx, float dy, float dt) {
    constexpr float sensitivity = 0.1f;

    m_yaw += dx * sensitivity;
    m_pitch -= dy * sensitivity;

    m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

    Update(targetPos, m_yaw, m_pitch, dt);
}

void Camera::Update(const glm::vec3& targetPos, float yawDegrees, float pitchDegrees, float dt) {
    glm::vec3 desiredPos    = ComputeDesiredPosition(targetPos, yawDegrees, pitchDegrees);
    glm::vec3 desiredTarget = ComputeDesiredTarget(targetPos, yawDegrees, pitchDegrees);

    if (!m_initialized || m_config.smoothSpeed <= 0.0f) {
        m_position = desiredPos;
        m_target = desiredTarget;
        m_initialized = true;
        return;
    }

    float alpha = 1.0f - std::exp(-m_config.smoothSpeed * std::max(dt, 0.0f));
    m_position = glm::mix(m_position, desiredPos, alpha);
    m_target   = glm::mix(m_target, desiredTarget, alpha);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_target, m_up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(
        glm::radians(m_config.fovDegrees),
        aspectRatio,
        m_config.nearPlane,
        m_config.farPlane
    );
}