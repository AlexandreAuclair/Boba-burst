#include "bb_trackObject.h"
#include "src/Utilities/utilities.h"

TrackObject::TrackObject(){}
TrackObject::TrackObject(int id_, Transform t_) : Actor(id_, t_) { }
TrackObject::~TrackObject(){}

JPH::BodyID TrackObject::InitPhysics(JPH::BodyInterface& bodyInterface){
    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::BoxShape(JPH::RVec3(0.5f, 0.5f, 0.5f)), glmToJPH(transform.position), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::BONUS_OBJECTS);

    sphere_settings.mIsSensor = true;
    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    return bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::DontActivate);

}

void TrackObject::Init(std::unique_ptr<EventBus> &inputBus, JPH::BodyInterface& bodyInterface) {
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

}

void TrackObject::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void TrackObject::Update(float dt){}
void TrackObject::Render(float dt){}

void TrackObject::onCollision(CollisionEvent &event) {

    if ( event.acteur1 != this->id && event.acteur2 != this->id ) return ;

    // traitement des différents effets (par ex boost etc avec un switch case)

}
