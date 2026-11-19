#include "../EventBus/bb_event.h"

class KeyPressedEvent : public Event
{
public:
	// GLFW devrait avoir un type qui nous serais utile ici pour savoir quel touche a été pressé
	KeyPressedEvent() {}
	/// @brief Événement de touche enfoncé avec son ID GLF pour savoir la touche
	/// @param id Numéro GLF corespondant
	KeyPressedEvent(int id)
	{
		keyId = id;
	}
	/// @brief Événement permetant d'envoyer la position de la souris en même temps que l'ID GLF pour savoir la touche
	/// @param id Numéro GLF corespondant
	/// @param mouseX X de la souris
	/// @param mouseY Y de la souris
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