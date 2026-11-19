#include "bb_player.h"
#include "string"
#include "src/Events/bb_gameMovementEvent.h"

Player::Player() {}
Player::Player(int id, std::unique_ptr<EventBus> &inputBus, GLFWwindow *window)
{
    // Valeur pour Speed et Position sont temporaire à des fin de test.
    // TODO: RETIRER LORSQUE PLAYER INSTANCIER CORRECTEMENT
    speed = 10;
    Actor::position = glm::vec3(0, 0, 0);
    Logger::Log("PLAYER CONSTRUCTOR #2 CALLED!");
    controlManager = InputManager(inputBus, window);
    Actor::id = id;
    inputBus->SubscribeToEvent<KeyPressedEvent>(this, &Player::Input);
    inputBus->SubscribeToEvent<PauseEvent>(this, &Player::HandlePauseEvent);
    Logger::Log("PLAYER CONSTRUCTOR #2 DONE!");

    busPtr = &inputBus;
}
Player::~Player() {}

JPH::BodyID Player::InitPhysics(JPH::BodyInterface &bodyInterface)
{
    return JPH::BodyID(0);
}

void Player::UpdateFromPhysics(const JPH::BodyInterface &bodyInterface)
{
}
/// @brief Réception de l'événement de pause. Détermine si le joueur doit ignorer les entrées ou non.
/// @param event PauseEvent avec True ou False
void Player::HandlePauseEvent(PauseEvent &event)
{
    isIgnoringInputs = event.isPaused;
}
/// @brief Réception des événements d'entrée. S'il s'agit d'un entrée pour déplacer le joueur, on garde la commande en mémoire pour l'Update
/// @param event KeyPressedEvent avec la touche a traité
void Player::Input(KeyPressedEvent &event)
{
    // Ignore toute les entrées quand nous ne voulons pas que le joueur réagisse
    if (isIgnoringInputs)
    {
        return;
    }

    Logger::Log("RECIVED KEYPRESS EVENT!");
    Logger::Log("-------- ");
    Logger::Log("REGISTERED KEY: " + std::to_string(event.keyId));

    if (event.keyId == GLFW_KEY_LEFT)
    {
        //leftKeyPressThisFrame = true;
        busPtr->get()->EmitEvent<GameMovementEvent>(this->bodyID, true, false);
        Logger::Log("Left key pressed");
    }
    if (event.keyId == GLFW_KEY_RIGHT)
    {
        //RightKeyPressThisFrame = true;
        busPtr->get()->EmitEvent<GameMovementEvent>(this->bodyID, false, true);
        Logger::Log("Right key pressed");
    }
    if (event.keyId == GLFW_MOUSE_BUTTON_LEFT)
    {
        Logger::Log("MOUSE X POS: " + std::to_string(event.mouseXPos));
        Logger::Log("MOUSE Y POS: " + std::to_string(event.mouseYPos));
    }

    Logger::Log("-------- ");
}
/// @brief Mise à jours de la position du joueur avec deltaTime selon la touche enregistrer dans Input
/// @param dt deltaTime
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