#include "bb_track.h"
#include "src/Utilities/utilities.h"

Track::Track() : nbSections(0) {
}

Track::Track(int id_, Transform t_, int nbSections_) : Actor(id_, t_), nbSections(nbSections_){}

Track::~Track(){

}

JPH::BodyID Track::InitPhysics(JPH::BodyInterface& bodyInterface){

    /*JPH::ConvexHullShapeSettings hull_settings(MeshToJoltVertices(*mesh), JPH::cDefaultConvexRadius);
    JPH::ShapeSettings::ShapeResult result = hull_settings.Create();
    if (result.HasError())
    {
        // Handle error: result.GetError().c_str()
        Logger::Log("pbm de création de la shape");
        throw;
    }

    JPH::RefConst<JPH::Shape> convex_shape = result.Get();*/

   // JPH::Vec3 pos = glmToJPH(transform.position)
  // Logger::Log("Transform position : x :" + std::to_string(pos.GetX()) + " y : "+ std::to_string(pos.GetY()) + " z : " + std::to_string(pos.GetZ()));

    JPH::MeshShapeSettings mesh_settings(MeshToJoltVertexList(*mesh), MeshToJoltTriangles(*mesh));
    JPH::ShapeSettings::ShapeResult result = mesh_settings.Create();

    if (result.HasError())
    {
        // Handle error: result.GetError().c_str()
        Logger::Log("pbm de création de la shape");
        throw;
    }

    JPH::RefConst<JPH::Shape> track_shape = result.Get();

    JPH::BodyCreationSettings trackSettings = BodyCreationSettings(track_shape, glmToJPH(transform.position), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);

    trackSettings.mUserData = static_cast<JPH::uint64>(id);
    
    return bodyInterface.CreateAndAddBody(trackSettings, JPH::EActivation::DontActivate);

}

void Track::Init(std::unique_ptr<EventBus> &inputBus, JPH::BodyInterface& bodyInterface) {
    bodyID = InitPhysics(bodyInterface);
    busPtr = &inputBus;

    inputBus->SubscribeToEvent<CollisionEvent>(this, &Track::onCollision); // le goal va faire ça aussi

    // Logger::Log("Actor id=" + std::to_string(id)
    //     + " invalide=" + std::to_string(bodyID.IsInvalid())
    //     + " ajoute=" + std::to_string(!bodyID.IsInvalid() && bodyInterface.IsAdded(bodyID))
    //     + " layer=" + std::to_string(!bodyID.IsInvalid() ? bodyInterface.GetObjectLayer(bodyID) : -1)
    //     + " actif=" + std::to_string(!bodyID.IsInvalid() && bodyInterface.IsActive(bodyID))
    //     + " pos=" + (bodyID.IsInvalid() ? std::string("-") :
    //         std::to_string(bodyInterface.GetCenterOfMassPosition(bodyID).GetX()) + ","
    //         + std::to_string(bodyInterface.GetCenterOfMassPosition(bodyID).GetY()) + ","
    //         + std::to_string(bodyInterface.GetCenterOfMassPosition(bodyID).GetZ())));


    //JPH::AABox b = bodyInterface.GetTransformedShape(bodyID).GetWorldSpaceBounds();
    //Logger::Log("id=" + std::to_string(id)
    //    + " Y min/max = " + std::to_string(b.mMin.GetY()) + " / " + std::to_string(b.mMax.GetY()));
}


void Track::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void Track::Update(float dt) {
    //check for collision
    UpdateFromPhysics(*sections->bodyInterface);
}

void Track::Render(float dt) {
    // call vulkain to transfer vertex and color data
}

void Track::onCollision(CollisionEvent &event) {}