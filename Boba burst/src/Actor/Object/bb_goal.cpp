#include "bb_goal.h"
#include "src/Utilities/utilities.h"
#include "../Character/bb_player.h"
#include "src/Events/bb_WinEvent.h"

// MARCHE PAS 


Goal::Goal() : Track() {}
Goal::Goal(int id_, Transform t_, int nbSections_) : Track(id_, t_, nbSections_) {}
Goal::~Goal() {}


void Goal::onCollision(CollisionEvent &event) {
    /*
    if (event.acteur1 != id && event.acteur2 != id) return;
    int otherId = (event.acteur1 == id) ? event.acteur2 : event.acteur1;

    auto& actors = GameManager::get().actorsById;
    auto it = actors.find(otherId);
    if (it == actors.end()) return;

    if (auto* p = dynamic_cast<Player*>(it->second))     // les bots sont sur le layer PLAYER aussi
        busPtr->get()->EmitEvent<WinEvent>(p->GetId());  // GetId() : accesseur public à ajouter

        */

}