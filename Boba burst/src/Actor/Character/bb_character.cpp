#include "bb_character.h"

Character::Character(){}

Character::~Character(){}

JPH::BodyID Character::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void Character::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void Character::Update(float dt) {}
void Character::Render(float dt) {}