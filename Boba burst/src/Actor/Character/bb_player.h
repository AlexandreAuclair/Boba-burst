#pragma once

#include "bb_character.h"

#include "bb_inputManager.h"
#include "../../EventBus/bb_eventBus.h"
#include "../../Game/bb_camera.h"

class Player : public Character {
public:
    Player();
    Player(int id_, Transform t_, Camera cam_);
    ~Player();

void UpdateFromPhysics(const JPH::BodyInterface &bodyInterface) override final;
private:
    JPH::BodyID InitPhysics(JPH::BodyInterface &bodyInterface) override final;
    
    /// @brief Accuser de réception d'un événement de la touche flèche gauche. Inputs vers Update
    bool leftKeyPressThisFrame = false;
    /// @brief Accuser de réception d'un événement de la touche flèche gauche. Inputs vers Update
    bool RightKeyPressThisFrame = false;
    void HandlePauseEvent(PauseEvent &event);
    /// @brief Killswitch quand le joueur ne doit pas déagir aux événements d'entrée
    bool isIgnoringInputs = false;

public:
    void SetControlManager(const InputManager& inputM) { controlManager = inputM; }
    void Init(std::unique_ptr<EventBus> &inputBus, JPH::BodyInterface& bodyInterface) override final;
    void Input(KeyPressedEvent &event);
    void Update(float dt) override final;
    void Render(float dt) override final;

    InputManager controlManager;
    std::unique_ptr<EventBus> *busPtr;
    Camera cam;
};
