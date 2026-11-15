#include "bb_inputManager.h"

void InputManager::CheckInput()
{
    int keys[] = {
        GLFW_KEY_ESCAPE
    };

    for (auto key : keys)
    {
        bool isKeyPressed = IsKeyPressed(key);

        if (isKeyPressed && !lastInputs[key])
        {
            busPtr->get()->EmitEvent<KeyPressedEvent>(key);
        }

        lastInputs[key] = isKeyPressed;
    }

    int clicks[] = {
        GLFW_MOUSE_BUTTON_LEFT,
        GLFW_MOUSE_BUTTON_RIGHT,
    };

    for (auto mouseClick : clicks)
    {
        bool isKeyPressed = IsMouseClick(mouseClick);

        if (isKeyPressed && !lastInputs[mouseClick])
        {
            busPtr->get()->EmitEvent<KeyPressedEvent>(mouseClick);
        }

        lastInputs[mouseClick] = isKeyPressed;
    }

    if (IsKeyPressed(GLFW_KEY_LEFT))
    {
        busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_LEFT);
    }

    if (IsKeyPressed(GLFW_KEY_RIGHT))
    {
        busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_RIGHT);
    }

}

bool InputManager::IsKeyPressed(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool InputManager::IsMouseClick(int key) const
{
    return glfwGetMouseButton(m_window, key) == GLFW_PRESS;
}

void InputManager::UpdateMouse()
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
