#include "bb_inputManager.h"

void InputManager::CheckInput()
{
    // Array des touches d'ont nous voulons restrindre le traitement à une fois par appuit
    int keys[] = {
        GLFW_KEY_TAB,
        GLFW_KEY_ESCAPE
    };

    // Pour chaque touche dans l'array, nous vérifions si elle est active.
    // Si c'est le cas, et que sont dernier état connus n'est pas actif, nous émétons l'événement corespondant à la touche.
    for (auto key : keys)
    {
        bool isKeyPressed = IsKeyPressed(key);

        if (isKeyPressed && !lastInputs[key])
        {
            busPtr->get()->EmitEvent<KeyPressedEvent>(key);
        }

        // Mémoire des dernier états des touches
        lastInputs[key] = isKeyPressed;
    }

    // Array des cliques de souris d'ont nous voulons restrindre le traitement à une fois par appuit
    int clicks[] = {
        GLFW_MOUSE_BUTTON_LEFT,
        GLFW_MOUSE_BUTTON_RIGHT,
    };

    // Pour chaque clique dans l'array, nous vérifions si elle est active.
    // Si c'est le cas, et que sont dernier état connus n'est pas actif, nous émétons l'événement corespondant à ce clique ainsi que ses coordonées.
    UpdateMouse();
    for (auto mouseClick : clicks)
    {
        bool isKeyPressed = IsMouseClick(mouseClick);

        if (isKeyPressed && !lastInputs[mouseClick])
        {

            busPtr->get()->EmitEvent<KeyPressedEvent>(mouseClick, m_lastMouseX, m_lastMouseY);
        }

        // Mémoire des dernier états des cliques
        lastInputs[mouseClick] = isKeyPressed;
    }

    // Si la touche est active, peut importe si elle est maintenu, nous émétons l'événement corespondant.
    if (IsKeyPressed(GLFW_KEY_LEFT))
    {
        busPtr->get()->EmitEvent<KeyPressedEvent>(GLFW_KEY_LEFT);
    }

    // Si la touche est active, peut importe si elle est maintenu, nous émétons l'événement corespondant.
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
