#include "bb_bonusObject.h"

BonusObject::BonusObject(){}
BonusObject::~BonusObject(){}

JPH::BodyID BonusObject::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void BonusObject::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void BonusObject::Update(float dt){}
void BonusObject::Render(float dt){}