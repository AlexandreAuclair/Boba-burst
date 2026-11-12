#include "bb_bot.h"

Bot::Bot(){}
Bot::~Bot(){}

JPH::BodyID Bot::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void Bot::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void Bot::Update(int dt) {}
void Bot::Render(int dt) {}