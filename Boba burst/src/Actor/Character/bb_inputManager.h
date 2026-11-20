#pragma once

#include "../../EventBus/bb_eventBus.h"
#include "../../Events/bb_keyPressedEvent.h"
#include "../../Events/bb_pauseEvent.h"
#include <GLFW/glfw3.h>

class InputManager
{
public:
	InputManager(std::unique_ptr<EventBus> &inputBus, GLFWwindow *window)
	{
		// Référence locale du bus des événements et de la fenêtes pour appel tardive
		busPtr = &inputBus;
		m_window = window;
	}
	InputManager() {};
	~InputManager() {};
	// Fonction principale qui vérifie les inputs et émets les événements si nécésaire
	void CheckInput();
	inline double GetMouseDeltaX() const { return m_mouseDeltaX; }
	inline double GetMouseDeltaY() const { return m_mouseDeltaY; }

private:
	GLFWwindow *m_window;
	std::unique_ptr<EventBus> *busPtr;

	double m_lastMouseX = 0.0;
	double m_lastMouseY = 0.0;

	double m_mouseDeltaX = 0.0;
	double m_mouseDeltaY = 0.0;

	bool m_firstMouse = true;

	// Mémoire des dernier états des touches et cliques pour éviter une répétition
	std::unordered_map<int, bool> lastInputs;

protected:
	bool IsKeyPressed(int key) const;
	bool IsMouseClick(int key) const;
	void UpdateMouse();

	
};