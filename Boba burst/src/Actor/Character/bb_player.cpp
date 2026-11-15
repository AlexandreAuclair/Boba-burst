#include "bb_player.h"

Player::Player() {}
Player::Player(int id, std::unique_ptr<EventBus> &inputBus)
{
    controlManager = InputManager(inputBus);
    inputBus->SubscribeToEvent<KeyPressedEvent>(this, &Player::Input);
    Actor::id = id;
}
Player::~Player() {}

JPH::BodyID Player::InitPhysics(JPH::BodyInterface &bodyInterface)
{
    return JPH::BodyID(0);
}

void Player::UpdateFromPhysics(const JPH::BodyInterface &bodyInterface)
{
}

void Player::Input(KeyPressedEvent &event) {}
void Player::Update(int dt) {}
void Player::Render(int dt) {}