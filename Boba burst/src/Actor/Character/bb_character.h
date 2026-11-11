#pragma once

#include "../bb_actor.h"

class Character : public Actor {
public:
    Character();
    ~Character();

	void InitPhysics(JPH::BodyInterface& bodyInterface) override;

	void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override;

private:
    float speed;
};