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


	//////////////////////////
	// GETTER d'info pour la gestion de l'UI

	// Vrai uniquement pendant la frame où le bouton gauche vient d'être enfoncé => Pour problème où input détecté plusieurs fois
	bool WasLeftMouseClicked() const
	{
		return m_leftMouseClicked;
	}

	// Récupérer la position absolue du curseur.
	void GetMousePosition(double& x, double& y) const
	{
		x = m_lastMouseX;
		y = m_lastMouseY;
	}

	// Dimensions de la fenêtre GLFW.
	void GetWindowSize(int& width, int& height) const
	{
		glfwGetWindowSize(m_window, &width, &height);
	}

	// Dimensions du framebuffer.
	void GetFramebufferSize(int& width, int& height) const
	{
		glfwGetFramebufferSize(m_window, &width, &height);
	}
	//////////////////////////

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

	// Appui détecté pendant frame courante
	bool m_leftMouseClicked = false;

protected:
	bool IsKeyPressed(int key) const;
	bool IsMouseClick(int key) const;
	void UpdateMouse();
	
};


