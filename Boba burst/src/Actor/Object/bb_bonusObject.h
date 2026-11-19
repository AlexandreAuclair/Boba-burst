#pragma once

#include "../bb_actor.h"
#include "ObjectTypes.h"
#include <GLFW/glfw3.h>


class BonusObject : public Actor{
public:
    BonusObject();
    BonusObject(int id_, Transform t_);
    ~BonusObject();
private:
    JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
    BonusObjectsType type;
public:
    void Init(std::unique_ptr<EventBus> &inputBus, GLFWwindow *window, JPH::BodyInterface& bodyInterface) override final;
    void Update(float dt) override final;
    void Render(float dt) override final;
    void onCollision(CollisionEvent &event) override;
private:

};