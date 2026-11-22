#pragma once

#include "../bb_actor.h"
#include "../../DataStruct/bb_section.h"
#include <GLFW/glfw3.h>

class Track : public Actor{
public:
    Track();
    Track(int id_, Transform t_, int nbSections);
    ~Track();
private:
    JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
public:
    void Init(std::unique_ptr<EventBus> &inputBus, JPH::BodyInterface& bodyInterface) override final;
    void Update(float dt) override final;
    void Render(float dt) override final;
    virtual void onCollision(CollisionEvent &event);

    std::unique_ptr<EventBus>* busPtr;

protected:
    int nbSections;
    std::unique_ptr<Section> sections;
};