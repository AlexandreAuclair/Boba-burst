#pragma once

#include "bb_character.h"
#include <src/EventBus/bb_eventBus.h>
#include "bb_inputManager.h"


class Player : public Character
{
public:
    Player();
    Player(int id, std::unique_ptr<EventBus> &inputBus, GLFWwindow *window, JPH::BodyInterface& bodyInterface);
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
    InputManager controlManager;
    void Input(KeyPressedEvent &event);
    void Update(float dt) override final;
    void Render(float dt) override final;
    std::unique_ptr<EventBus> *busPtr;
};