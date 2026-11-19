#pragma once

#include "../bb_actor.h"

class Character : public Actor
{
public:
    Character();
    ~Character();

protected:
    JPH::BodyID InitPhysics(JPH::BodyInterface &bodyInterface) override;
    void UpdateFromPhysics(const JPH::BodyInterface &bodyInterface) override;
    void onCollision(CollisionEvent &event) override;
    float speed;

public:
    void Update(float dt) override;
    void Render(float dt) override;

private:
};