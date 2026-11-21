#pragma once

#include "../bb_actor.h"
#include "../../DataStruct/bb_section.h"

class Obstacle : public Actor{
public:
    Obstacle();
    ~Obstacle();

private:
    JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
public:
    void Init(std::unique_ptr<EventBus>& inputBus, JPH::BodyInterface& bodyInterface) override final;
    void Update(float dt) override final;
    void Render(float dt) override final;
    void onCollision(CollisionEvent &event) override;   
private:
};