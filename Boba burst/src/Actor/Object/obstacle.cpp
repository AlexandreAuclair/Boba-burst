#include "obstacle.h"
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include "../../Game/PhysicsWorld.h"
#include "src/Utilities/utilities.h"

Obstacle::Obstacle(int id_, Transform t_): Actor(id_,t_){}
Obstacle::~Obstacle(){}

JPH::BodyID Obstacle::InitPhysics(JPH::BodyInterface& bodyInterface){
    JPH::BodyCreationSettings sphere_settings = BodyCreationSettings(new JPH::BoxShape(JPH::RVec3(transform.scale.x, transform.scale.y, transform.scale.z)), glmToJPH(transform.position), glmEulerToJPHQuat(transform.rotation), JPH::EMotionType::Static, Layers::OBSTACLES);

    sphere_settings.mIsSensor = false;
    sphere_settings.mUserData = static_cast<JPH::uint64>(id);
    return bodyInterface.CreateAndAddBody(sphere_settings, JPH::EActivation::DontActivate);

}

void Obstacle::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    auto pos = bodyInterface.GetPosition(this->bodyID);
    this->transform.position = glm::vec3(pos.GetX(), -(pos.GetY()), pos.GetZ());
}


void Obstacle::Init(std::unique_ptr<EventBus>& inputBus, JPH::BodyInterface& bodyInterface) {
    bodyID = InitPhysics(bodyInterface);

}
void Obstacle::Update(float dt){}
void Obstacle::Render(float dt){}

void Obstacle::onCollision(CollisionEvent &event) {

    if ( event.acteur1 != this->id && event.acteur2 != this->id ) return ;

    // traitement des différents effets (par ex boost etc avec un switch case)

}