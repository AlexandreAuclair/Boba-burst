#pragma once

#include "bb_character.h"

class Bot : public Character {
public:
    Bot();
    Bot(int id_, Transform t_);
    ~Bot();

private:
	JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
	void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
    void onCollision(CollisionEvent &event) override final;
public:
    void Update(float dt) override final;
    void Render(float dt) override final;
};