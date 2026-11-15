#pragma once

#include "bb_character.h"
#include <src/EventBus/bb_eventBus.h>
#include "bb_inputManager.h"

class Player : public Character
{
public:
    Player();
    Player(int id, std::unique_ptr<EventBus> &inputBus);
    ~Player();

private:
    JPH::BodyID InitPhysics(JPH::BodyInterface &bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface &bodyInterface) override final;

public:
    InputManager controlManager;
    void Input(KeyPressedEvent &event);
    void Update(int dt) override final;
    void Render(int dt) override final;
};