#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <GLFW/glfw3.h>

class Controller {
public:
	Controller() = default;

	//Controller(GLFWwindow* window);

	~Controller() = default;
	bool IsKeyPressed(int key) const;
	void UpdateMouse();


	// Setter window au lieu du constructeur paramétrique
	void SetWindow(GLFWwindow* newWindow) { m_window = newWindow;}

	inline double GetMouseDeltaX() const { return m_mouseDeltaX; }
	inline double GetMouseDeltaY() const { return m_mouseDeltaY; }

private:
	GLFWwindow* m_window = nullptr;
	double m_lastMouseX = 0.0;
	double m_lastMouseY = 0.0;

	double m_mouseDeltaX = 0.0;
	double m_mouseDeltaY = 0.0;

	bool m_firstMouse = true;
};

#endif