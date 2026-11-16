#include "bb_bonusObject.h"
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include "../../Game/PhysicsWorld.h"

BonusObject::BonusObject(){}
BonusObject::~BonusObject(){}

JPH::BodyID BonusObject::InitPhysics(JPH::BodyInterface& bodyInterface){

    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::SphereShape(0.5f), JPH::RVec3(0.0_r, 0.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::BONUS_OBJECTS);

    sphere_settings.mIsSensor = true;
    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    return bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::DontActivate);
}

void BonusObject::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void BonusObject::Update(int dt){}
void BonusObject::Render(int dt){}