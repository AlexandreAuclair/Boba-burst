#pragma once

#include "../bb_actor.h"
#include "../../DataStruct/bb_section.h"

class Track : public Actor{
public:
    Track();
    Track(int id, int nbSections);
    ~Track();
private:
    JPH::BodyID InitPhysics(JPH::BodyInterface& bodyInterface) override final;
    void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override final;
public:
    void Update(int dt) override final;
    void Render(int dt) override final;


private:
    int nbSections;
    std::unique_ptr<Section> sections;
};