#include "../EventBus/bb_event.h"

class CollisionEvent : public Event {
	public:
        // GLFW devrait avoir un type qui nous serais utile ici pour savoir quel touche a été pressé
		CollisionEvent(int _acteur1, int _acteur2) : acteur1{_acteur1}, acteur2{_acteur2} {

		}

		int acteur1, acteur2;
};