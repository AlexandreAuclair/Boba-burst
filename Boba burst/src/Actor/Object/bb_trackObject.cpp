#include "bb_trackObject.h"

TrackObject::TrackObject(){}
TrackObject::TrackObject(int id_, Transform t_) : Actor(id_, t_) { }
TrackObject::~TrackObject(){}

JPH::BodyID TrackObject::InitPhysics(JPH::BodyInterface& bodyInterface){
    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::BoxShape(JPH::RVec3(0.5f, 0.5f, 0.5f)), JPH::RVec3(0.0_r, 0.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::BONUS_OBJECTS);

    sphere_settings.mIsSensor = true;
    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    return bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::DontActivate);

}

void TrackObject::Init(std::unique_ptr<EventBus> &inputBus, GLFWwindow *window, JPH::BodyInterface& bodyInterface) {
    InitPhysics(bodyInterface);
}

void TrackObject::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void TrackObject::Update(float dt){}
void TrackObject::Render(float dt){}

void TrackObject::onCollision(CollisionEvent &event) {

    if ( event.acteur1 != this->id && event.acteur2 != this->id ) return ;

    // traitement des différents effets (par ex boost etc avec un switch case)

}
