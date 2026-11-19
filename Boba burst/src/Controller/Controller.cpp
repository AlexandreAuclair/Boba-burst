#include "Controller.h"

/*Controller::Controller(GLFWwindow* window) {
    m_window = window;
}*/

bool Controller::IsKeyPressed(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

void Controller::UpdateMouse()
{
    double mouseX;
    double mouseY;

    glfwGetCursorPos(m_window, &mouseX, &mouseY);

    if (m_firstMouse)
    {
        m_lastMouseX = mouseX;
        m_lastMouseY = mouseY;
        m_firstMouse = false;
    }

    m_mouseDeltaX = mouseX - m_lastMouseX;
    m_mouseDeltaY = mouseY - m_lastMouseY;

    m_lastMouseX = mouseX;
    m_lastMouseY = mouseY;
}