#pragma once

#include "bb_track.h"


class Goal : public Track{
public:
    Goal();
    Goal(int id_, Transform t_, int nbSections);
    ~Goal();
    void onCollision(CollisionEvent &event) override ;
    bool goalWasReach;
};