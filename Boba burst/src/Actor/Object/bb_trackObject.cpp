#include "bb_trackObject.h"

TrackObject::TrackObject(){}
TrackObject::~TrackObject(){}

JPH::BodyID TrackObject::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void TrackObject::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void TrackObject::Update(int dt){}
void TrackObject::Render(int dt){}