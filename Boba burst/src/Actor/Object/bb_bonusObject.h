#pragma once

#include "../bb_actor.h"

class BonusObject : public Actor{
public:
    BonusObject();
    ~BonusObject();
private:
    JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
public:
    void Update(int dt) override final;
    void Render(int dt) override final;
private:

};