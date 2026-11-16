#include "obstacle.h"
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include "../../Game/PhysicsWorld.h"


Obstacle::Obstacle(){}
Obstacle::~Obstacle(){}

JPH::BodyID Obstacle::InitPhysics(JPH::BodyInterface& bodyInterface){
    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::BoxShape(JPH::RVec3(0.5f, 0.5f, 0.5f)), JPH::RVec3(0.0_r, 0.0_r, 0.0_r), JPH::Quat::sIdentity(), JPH::EMotionType::Dynamic, Layers::BONUS_OBJECTS);

    sphere_settings.mIsSensor = false;
    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    return bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::DontActivate);

}

void Obstacle::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void Obstacle::Update(int dt){}
void Obstacle::Render(int dt){}