#include "bb_player.h"

Player::Player(){}
Player::~Player(){}

JPH::BodyID Player::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void Player::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void Player::Input() {}
void Player::Update(int dt) {}
void Player::Render(int dt) {}