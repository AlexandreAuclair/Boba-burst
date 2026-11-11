#include "../EventBus/bb_event.h"

class KeyPressedEvent : public Event {
	public:
        // GLFW devrait avoir un type qui nous serais utile ici pour savoir quel touche a été pressé
		KeyPressedEvent() {}
};