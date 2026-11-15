#include "../EventBus/bb_event.h"

class KeyPressedEvent : public Event
{
public:
	// GLFW devrait avoir un type qui nous serais utile ici pour savoir quel touche a été pressé
	KeyPressedEvent() {}
	KeyPressedEvent(int id)
	{
		keyId = id;
	}
	KeyPressedEvent(int id, int mouseX, int mouseY)
	{
		keyId = id;
		mouseXPos = mouseX;
		mouseYPos = mouseY;
	}
	int keyId;
	int mouseXPos;
	int mouseYPos;
};