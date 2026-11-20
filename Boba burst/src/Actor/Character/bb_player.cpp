#include "bb_player.h"

Player::Player() {}
Player::Player(int id_, Transform t_, Camera cam_) : Character(id_, t_){
    cam = Camera(cam_);
}

void Player::Init(std::unique_ptr<EventBus> &inputBus, GLFWwindow *window, JPH::BodyInterface& bodyInterface) {
    Logger::Log("PLAYER CONSTRUCTOR #2 CALLED!");
    controlManager = InputManager(inputBus, window);
    Actor::id = id;
    inputBus->SubscribeToEvent<KeyPressedEvent>(this, &Player::Input);
    inputBus->SubscribeToEvent<PauseEvent>(this, &Player::HandlePauseEvent);
    Logger::Log("PLAYER CONSTRUCTOR #2 DONE!");

    InitPhysics(bodyInterface);

    busPtr = &inputBus;
}

Player::~Player() {}

JPH::BodyID Player::InitPhysics(JPH::BodyInterface &bodyInterface)
{
    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::BoxShape(JPH::RVec3(0.5f, 0.5f, 0.5f)), JPH::RVec3(0.0_r, 0.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::BONUS_OBJECTS);

    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    this->bodyID = bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::DontActivate);
    return this->bodyID;
}


void Player::UpdateFromPhysics(const JPH::BodyInterface &bodyInterface)
{
    auto pos = bodyInterface.GetPosition(this->bodyID);
    this->transform.position = glm::vec3(pos.GetX(), -(pos.GetY()), pos.GetZ());

    auto rot = bodyInterface.GetRotation(this->bodyID).GetXYZ();
    this->transform.rotation = glm::vec3(rot[0], -(rot[1]), rot[2]);
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

    if (event.keyId == GLFW_KEY_TAB) {
        cam.ToggleMode();
    }

    Logger::Log("-------- ");
}

/// @brief Mise à jours de la position du joueur avec deltaTime selon la touche enregistrer dans Input
/// @param dt deltaTime
void Player::Update(float dt)
{
    if (!isIgnoringInputs) {
        // Exemple : rotation horizontale via flèches gauche/droite (à adapter à ton système)
        if (leftKeyPressThisFrame)  transform.rotation.z += 90.0f * dt;
        if (RightKeyPressThisFrame) transform.rotation.z -= 90.0f * dt;
    }
    cam.Update(GetTransform().position, transform.rotation.z, transform.rotation.x, dt);
    // UpdateFromPhysics();
}
void Player::Render(float dt) {}
