#include "bb_inputManager.h"

void InputManager::CheckInput()
{
    if (IsKeyPressed(GLFW_KEY_A))
    {
        busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_A);
    }
    if (IsKeyPressed(GLFW_KEY_D))
    {
        busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_D);
    }
}

bool InputManager::IsKeyPressed(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}
