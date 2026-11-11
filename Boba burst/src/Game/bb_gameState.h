#pragma once

#include "../Logger/Logger.h"

enum class GameState {
    Menu,
    Playing,
    Pause,
    GameOver,
    Option,
    Quitting
};

class GameStateMachine {
private:
    GameState currentState;

    // Changement d'état brut (avec gestion optionnelle des entrées/sorties d'états)
    void ChangeState(GameState newState) {
        // Vous pouvez ajouter ici des logs ou des actions de transition (ex: charger une scène)
        currentState = newState;
    }

public:
    GameStateMachine();

    // Obtenir l'état actuel
    GameState GetCurrentState() const;

    // --- Événements et Transitions selon vos règles ---

    // Depuis Menu
    void TriggerPlay();

    void TriggerOption();

    void TriggerQuit();

    // Depuis Playing
    void TriggerPause();

    void TriggerGameOver();

    // Depuis Pause
    void TriggerResume();

    // Depuis Game Over
    void TriggerRestart();

    // Retours communs vers le Menu (Option, Pause, Game Over -> Menu)
    void TriggerBackToMenu();
};