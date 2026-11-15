#pragma once

#include "../../EventBus/bb_eventBus.h"
#include "../../Events/bb_keyPressedEvent.h"
#include "../../Events/bb_pauseEvent.h"
#include <GLFW/glfw3.h>

enum class AvailableKeys
{
	none, left, right, esc
};

class InputManager
{
public:
	InputManager(std::unique_ptr<EventBus> &inputBus, GLFWwindow *window)
	{
		busPtr = &inputBus;
		m_window = window;
	}
	InputManager() {};
	~InputManager() {};
	void CheckInput();

private:
	GLFWwindow *m_window;
	std::unique_ptr<EventBus> *busPtr;

	double m_lastMouseX = 0.0;
	double m_lastMouseY = 0.0;

	double m_mouseDeltaX = 0.0;
	double m_mouseDeltaY = 0.0;

	bool m_firstMouse = true;

	AvailableKeys lastInput = AvailableKeys::none;

protected:
	bool IsKeyPressed(int key) const;
	void UpdateMouse();

	inline double GetMouseDeltaX() const { return m_mouseDeltaX; }
	inline double GetMouseDeltaY() const { return m_mouseDeltaY; }
};