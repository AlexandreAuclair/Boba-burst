#include "bb_player.h"
#include "string"

Player::Player() {}
Player::Player(int id, std::unique_ptr<EventBus> &inputBus, GLFWwindow *window)
{
    controlManager = InputManager(inputBus, window);
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

void Player::Input(KeyPressedEvent &event)
{
    Logger::Log(std::to_string(event.keyId));
}
void Player::Update(int dt) {}
void Player::Render(int dt) {}