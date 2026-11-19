#include "bb_bot.h"

Bot::Bot(){}
Bot::Bot(int id_, Transform t_) : Character(id_,t_){}
Bot::~Bot(){}

JPH::BodyID Bot::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void Bot::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void Bot::Update(float dt) {}
void Bot::Render(float dt) {}

void Bot::onCollision(CollisionEvent &event) {}