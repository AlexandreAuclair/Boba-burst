#include "bb_player.h"
#include "../../Utilities/utilities.h"

Player::Player() {}
Player::Player(int id_, Transform t_, Camera cam_) : Character(id_, t_){
    cam = Camera(cam_);
}

void Player::Init(std::unique_ptr<EventBus> &inputBus, JPH::BodyInterface& bodyInterface) {

    inputBus->SubscribeToEvent<KeyPressedEvent>(this, &Player::Input);
    inputBus->SubscribeToEvent<PauseEvent>(this, &Player::HandlePauseEvent);

    bodyID = InitPhysics(bodyInterface);

    // Logger::Log("Actor id=" + std::to_string(id)
    //     + " invalide=" + std::to_string(bodyID.IsInvalid())
    //     + " ajoute=" + std::to_string(!bodyID.IsInvalid() && bodyInterface.IsAdded(bodyID))
    //     + " layer=" + std::to_string(!bodyID.IsInvalid() ? bodyInterface.GetObjectLayer(bodyID) : -1)
    //     + " actif=" + std::to_string(!bodyID.IsInvalid() && bodyInterface.IsActive(bodyID))
    //     + " pos=" + (bodyID.IsInvalid() ? std::string("-") :
    //         std::to_string(bodyInterface.GetCenterOfMassPosition(bodyID).GetX()) + ","
    //         + std::to_string(bodyInterface.GetCenterOfMassPosition(bodyID).GetY()) + ","
    //         + std::to_string(bodyInterface.GetCenterOfMassPosition(bodyID).GetZ())));

    busPtr = &inputBus;


    JPH::AABox b = bodyInterface.GetTransformedShape(bodyID).GetWorldSpaceBounds();
    Logger::Log("id=" + std::to_string(id)
        + " Y min/max = " + std::to_string(b.mMin.GetY()) + " / " + std::to_string(b.mMax.GetY()));
}

Player::~Player() {}

JPH::BodyID Player::InitPhysics(JPH::BodyInterface &bodyInterface)
{ // glmToJPH(transform.position)
    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::SphereShape(0.5f), glmToJPH(transform.position), glmEulerToJPHQuat(transform.rotation), JPH::EMotionType::Dynamic, Layers::PLAYER);

    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    return  bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::Activate);

}


void Player::UpdateFromPhysics(const JPH::BodyInterface &bodyInterface)
{
    auto pos = bodyInterface.GetPosition(this->bodyID);
    this->transform.position = glm::vec3(pos.GetX(), -(pos.GetY()), pos.GetZ());

    //auto rot = bodyInterface.GetRotation(this->bodyID).GetXYZ();
    //this->transform.rotation = glm::vec3(rot[0], -(rot[1]), rot[2]);

    auto rot = bodyInterface.GetRotation(this->bodyID);
    this->transform.rotation = JPHQuatToGlmEuler(rot);
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
        busPtr->get()->EmitEvent<GameMovementEvent>(this->bodyID, true, false);
        Logger::Log("Left key pressed");
    }
    if (event.keyId == GLFW_KEY_RIGHT)
    {
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
        const double deltaX = controlManager.GetMouseDeltaX();
        const double deltaY = controlManager.GetMouseDeltaY();
        if (deltaX != 0 || deltaY != 0) {
            cam.ProcessMouseMovement(deltaX, deltaY);
        }
        cam.Update(GetTransform().position, dt);
    }
    // UpdateFromPhysics();
}
void Player::Render(float dt) {}
