#pragma once

#include "bb_character.h"

class Bot : public Character {
public:
    Bot();
    ~Bot();

private:
	JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
	void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
public:
    void Update(float dt) override final;
    void Render(float dt) override final;
};