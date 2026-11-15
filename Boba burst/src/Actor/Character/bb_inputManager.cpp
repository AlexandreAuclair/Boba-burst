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
}

bool InputManager::IsKeyPressed(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}
