#pragma once

#include "bb_character.h"

class Player : public Character {
public:
    Player();
    ~Player();
private:
	JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
	void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;

public:
    void Input();
    void Update(int dt) override final;
    void Render(int dt) override final;
};