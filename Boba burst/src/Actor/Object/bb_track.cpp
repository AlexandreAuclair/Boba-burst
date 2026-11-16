#include "bb_track.h"

Track::Track() : nbSections(0) {
}

Track::Track(int id_, Transform t_, int nbSections_) : Actor(id_, t_), nbSections(nbSections_){}

Track::~Track(){

}

JPH::BodyID Track::InitPhysics(JPH::BodyInterface& bodyInterface){
    return JPH::BodyID(0);
}

void Track::UpdateFromPhysics(const JPH::BodyInterface& bodyInterface){
    
}

void Track::Update(int dt) {
    //check for collision
    UpdateFromPhysics(*sections->bodyInterface);
}

void Track::Render(int dt) {
    // call vulkain to transfer vertex and color data
}