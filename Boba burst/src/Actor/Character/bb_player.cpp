#include "bb_player.h"
#include "string"

Player::Player() {}
Player::Player(int id, std::unique_ptr<EventBus> &inputBus, GLFWwindow *window)
{
    Actor::position = glm::vec3(0,0,0);
    Logger::Log("PLAYER CONSTRUCTOR #2 CALLED!");
    controlManager = InputManager(inputBus, window);
    Actor::id = id;
    inputBus->SubscribeToEvent<KeyPressedEvent>(this, &Player::Input);
    Logger::Log("PLAYER CONSTRUCTOR #2 DONE!");
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
    Logger::Log("RECIVED KEYPRESS EVENT!");
    Logger::Log("-------- ");
    Logger::Log("REGISTERED KEY: " + std::to_string(event.keyId));

    if (event.keyId == GLFW_KEY_LEFT)
    {
        Logger::Log("--PREV POS-- ");
        Logger::Log("X: " + std::to_string(Actor::position.x));
        Logger::Log("Y: " + std::to_string(Actor::position.y));
        Logger::Log("Z: " + std::to_string(Actor::position.z));

        Actor::position = glm::vec3(Actor::position.x + 0.1f, Actor::position.y, Actor::position.z);

        Logger::Log("--NEW POS-- ");
        Logger::Log("X: " + std::to_string(Actor::position.x));
        Logger::Log("Y: " + std::to_string(Actor::position.y));
        Logger::Log("Z: " + std::to_string(Actor::position.z));
    }
    //compteur++;
    Logger::Log("-------- ");
}
void Player::Update(int dt) {}
void Player::Render(int dt) {}