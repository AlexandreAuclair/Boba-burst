#include "bb_player.h"
#include "string"

Player::Player() {}
Player::Player(int id, std::unique_ptr<EventBus> &inputBus, GLFWwindow *window)
{
    speed = 10;
    Actor::position = glm::vec3(0, 0, 0);
    Logger::Log("PLAYER CONSTRUCTOR #2 CALLED!");
    controlManager = InputManager(inputBus, window);
    Actor::id = id;
    inputBus->SubscribeToEvent<KeyPressedEvent>(this, &Player::Input);
    inputBus->SubscribeToEvent<PauseEvent>(this, &Player::HandlePauseEvent);
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

void Player::HandlePauseEvent(PauseEvent &event)
{
    isIgnoringInputs = event.isPaused;
}

void Player::Input(KeyPressedEvent &event)
{
    if (isIgnoringInputs)
    {
        return;
    }

    Logger::Log("RECIVED KEYPRESS EVENT!");
    Logger::Log("-------- ");
    Logger::Log("REGISTERED KEY: " + std::to_string(event.keyId));

    if (event.keyId == GLFW_KEY_LEFT)
    {
        leftKeyPressThisFrame = true;
    }
    if (event.keyId == GLFW_KEY_RIGHT)
    {
        RightKeyPressThisFrame = true;
    }
    if (event.keyId == GLFW_MOUSE_BUTTON_LEFT)
    {
        Logger::Log("MOUSE X POS: " + std::to_string(event.mouseXPos));
        Logger::Log("MOUSE Y POS: " + std::to_string(event.mouseYPos));
    }

    Logger::Log("-------- ");
}
void Player::Update(float dt)
{
    if (leftKeyPressThisFrame)
    {
        Logger::Log("--PREV POS-- ");
        Logger::Log("X: " + std::to_string(Actor::position.x));
        Logger::Log("Y: " + std::to_string(Actor::position.y));
        Logger::Log("Z: " + std::to_string(Actor::position.z));

        Actor::position = glm::vec3((Actor::position.x += dt * speed), Actor::position.y, Actor::position.z);

        Logger::Log("--NEW POS-- ");
        Logger::Log("X: " + std::to_string(Actor::position.x));
        Logger::Log("Y: " + std::to_string(Actor::position.y));
        Logger::Log("Z: " + std::to_string(Actor::position.z));

        leftKeyPressThisFrame = false;
    }
    if (RightKeyPressThisFrame)
    {
        Logger::Log("--PREV POS-- ");
        Logger::Log("X: " + std::to_string(Actor::position.x));
        Logger::Log("Y: " + std::to_string(Actor::position.y));
        Logger::Log("Z: " + std::to_string(Actor::position.z));

        Actor::position = glm::vec3((Actor::position.x -= dt * speed), Actor::position.y, Actor::position.z);

        Logger::Log("--NEW POS-- ");
        Logger::Log("X: " + std::to_string(Actor::position.x));
        Logger::Log("Y: " + std::to_string(Actor::position.y));
        Logger::Log("Z: " + std::to_string(Actor::position.z));

        RightKeyPressThisFrame = false;
    }
    // UpdateFromPhysics();
}
void Player::Render(float dt) {}