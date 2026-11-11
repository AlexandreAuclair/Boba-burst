#pragma once

#include "../bb_actor.h"
#include "../../DataStruct/bb_section.h"

class BonusObject : public Actor{
public:
    BonusObject();
    ~BonusObject();

    void InitPhysics(JPH::BodyInterface& bodyInterface) override;
    void UpdateFromPhysics(const JPH::BodyInterface& bodyInterface) override;

private:
    int nbSections;
    std::unique_ptr<Section> sections;
};