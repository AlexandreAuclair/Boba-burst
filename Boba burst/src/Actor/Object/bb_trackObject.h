#pragma once

#include "../bb_actor.h"
#include "../../DataStruct/bb_section.h"

class TrackObject : public Actor{
public:
    TrackObject();
    TrackObject(int id_, Transform t_);
    ~TrackObject();

private:
    JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
public:
    void Update(float dt) override final;
    void Render(float dt) override final;
    void onCollision(CollisionEvent &event) override;

private:
    int nbSections;
    std::unique_ptr<Section> sections;
};