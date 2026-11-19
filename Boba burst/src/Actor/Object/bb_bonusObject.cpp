#include "bb_bonusObject.h"
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include "../../Game/PhysicsWorld.h"

BonusObject::BonusObject(){}
BonusObject::BonusObject(int id_, Transform t_) : Actor(id_, t_){}
BonusObject::~BonusObject(){}

JPH::BodyID BonusObject::InitPhysics(JPH::BodyInterface& bodyInterface){

    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0_r, 0.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::BONUS_OBJECTS);

    sphere_settings.mIsSensor = true;
    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    return bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::DontActivate);
}

void BonusObject::Init(std::unique_ptr<EventBus> &inputBus, GLFWwindow *window, JPH::BodyInterface& bodyInterface) {
    InitPhysics(bodyInterface);
}

void BonusObject::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void BonusObject::Update(float dt){}
void BonusObject::Render(float dt){}

void BonusObject::onCollision(CollisionEvent &event) {

    if ( event.acteur1 != this->id && event.acteur2 != this->id ) return ;

    // traitement des différents effets (par ex boost etc avec un switch case)

}
