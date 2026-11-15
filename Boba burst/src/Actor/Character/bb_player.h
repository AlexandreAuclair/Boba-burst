#pragma once

#include "bb_character.h"
#include <src/EventBus/bb_eventBus.h>
#include "bb_inputManager.h"

class Player : public Character
{
public:
    Player();
    Player(int id, std::unique_ptr<EventBus> &inputBus, GLFWwindow *window);
    ~Player();

private:
    JPH::BodyID InitPhysics(JPH::BodyInterface &bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface &bodyInterface) override final;
    bool leftKeyPressThisFrame = false;
    bool RightKeyPressThisFrame = false;
    void HandlePauseEvent(PauseEvent &event);
    bool isIgnoringInputs = false;

public:
    InputManager controlManager;
    void Input(KeyPressedEvent &event);
    void Update(float dt) override final;
    void Render(float dt) override final;
};