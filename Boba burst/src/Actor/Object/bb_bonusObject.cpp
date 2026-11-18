#include "bb_bonusObject.h"

BonusObject::BonusObject(){}
BonusObject::BonusObject(int id_, Transform t_) : Actor(id_, t_){}
BonusObject::~BonusObject(){}

JPH::BodyID BonusObject::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void BonusObject::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void BonusObject::Update(int dt){}
void BonusObject::Render(int dt){}