#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

enum class CameraMode
{
    LevelView,  // Orbite autour de la cible
    Freecam     // Déplacement libre
};

class Camera
{
public:
    Camera(
        glm::vec3 position = glm::vec3(255.0f, 255.0f, -255.0f),
        glm::vec3 target = glm::vec3(0.0f),
        glm::vec3 worldUp = glm::vec3(0.0f, 0.0f, 1.0f)
    ) ;

    glm::mat4 GetViewMatrix() const;

    glm::mat4 GetProjMatrix(float width, float height) const;

    const glm::vec3& GetPosition() const;

    const glm::vec3& GetTarget() const ;

    CameraMode GetMode() const ;
private:
    void SetMode(CameraMode mode)  ;
public:
    void ToggleMode()  ;

    void ProcessMouseMovement(float dx, float dy)  ;

    void MoveForward(float amount)  ;

    void MoveRight(float amount)  ;

    void MoveUp(float amount)  ;

    void ResetCurrentCam();

private:
    CameraMode m_mode = CameraMode::LevelView;

    // LEVELCAM =======

    glm::vec3 m_position_l;
    glm::vec3 m_target_l;
    glm::vec3 m_worldUp_l;

    glm::vec3 m_front_l;
    glm::vec3 m_right_l;
    glm::vec3 m_up_l;

    float m_yaw_l = 0.0f;
    float m_pitch_l = -90.0f;
    float m_distance_l = 1000.0f;

    // FREECAM

    glm::vec3 m_position_f;
    glm::vec3 m_target_f;
    glm::vec3 m_worldUp_f;

    glm::vec3 m_front_f;
    glm::vec3 m_right_f;
    glm::vec3 m_up_f;

    float m_yaw_f = 0.0f;
    float m_pitch_f = 0.0f;
    float m_distance_f = 1.0f;

    void UpdateAnglesFromDirection(const glm::vec3& direction)  ;

    void UpdateVectors()  ;
};