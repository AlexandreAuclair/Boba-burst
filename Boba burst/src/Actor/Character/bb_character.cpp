#include "bb_character.h"

Character::Character(){}

Character::~Character(){}

JPH::BodyID Character::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void Character::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}
void Character::Init(std::unique_ptr<EventBus> &inputBus, JPH::BodyInterface& bodyInterface) {
}
void Character::Update(float dt){
    if(followTarget)
        transform.position = followTarget->GetTransform().position;
}
void Character::Render(float dt) {}
void Character::onCollision(CollisionEvent &event) {}