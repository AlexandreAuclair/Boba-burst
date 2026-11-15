#include "bb_inputManager.h"

void InputManager::CheckInput()
{
    if (IsKeyPressed(GLFW_KEY_RIGHT))
    {
        busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_RIGHT);
    }
    if (IsKeyPressed(GLFW_KEY_LEFT))
    {
        busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_LEFT);
    }
    if (IsKeyPressed(GLFW_KEY_ESCAPE))
    {
        if (lastInput != AvailableKeys::esc)
        {
            lastInput = AvailableKeys::esc;
            busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_ESCAPE);
        }
    }
    else
    {
        lastInput = AvailableKeys::none;
    }
}

bool InputManager::IsKeyPressed(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}
