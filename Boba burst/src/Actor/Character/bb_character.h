#pragma once

#include "../bb_actor.h"

class Character : public Actor {
public:
    Character();
    ~Character();

protected:
	JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override;
	void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override;
public:
    void Update(int dt) override;
    void Render(int dt) override;
private:
    float speed;
};