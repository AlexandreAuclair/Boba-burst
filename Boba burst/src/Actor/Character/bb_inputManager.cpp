#include "bb_inputManager.h"

void InputManager::CheckInput()
{

    // ==================================================
    // GESTION DES CLICS DE TOUCHES
    // ==================================================

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

    // ==================================================
    // GESTION DES CLICS DE SOURIS
    // ==================================================

   // Array des cliques de souris d'ont nous voulons restrindre le traitement à une fois par appuit
    int clicks[] = {
        GLFW_MOUSE_BUTTON_LEFT,
        GLFW_MOUSE_BUTTON_RIGHT,
    };

    // Réinitialiser le clic gauche pour la frame courante.
    m_leftMouseClicked = false;

    // Mettre à jour la position et les déplacements souris.
    UpdateMouse();

    for (auto mouseButton : clicks)
    {
        // État actuel du bouton.
        const bool isPressed = IsMouseClick(mouseButton);

        // Nouvel appui : RELEASE -> PRESS.
        const bool clicked = isPressed && !lastInputs[mouseButton];

        if (clicked)
        {
            // Conserver l'émission d'événement existante.
            busPtr->get()->EmitEvent<KeyPressedEvent>(
                mouseButton,
                m_lastMouseX,
                m_lastMouseY
            );

            // Mémoriser uniquement le clic gauche pour le traitement du menu de pause
            if (mouseButton == GLFW_MOUSE_BUTTON_LEFT)
            {
                m_leftMouseClicked = true;
            }
        }

        // Mémoriser l'état RÉEL du bouton pour la prochaine frame
        lastInputs[mouseButton] = isPressed;
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
