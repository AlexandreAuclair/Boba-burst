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
    GameStateMachine() : currentState(GameState::Menu) {}

    // Obtenir l'état actuel
    GameState GetCurrentState() const { return currentState; }

    // --- Événements et Transitions selon vos règles ---

    // Depuis Menu
    void TriggerPlay() {
        if (currentState == GameState::Menu) {
            ChangeState(GameState::Playing);
            Logger::Log("Game state is now playing");
        }
    }

    void TriggerOption() {
        if (currentState == GameState::Menu) {
            ChangeState(GameState::Option);
            Logger::Log("Game state is now Option");
        }
    }

    void TriggerQuit() {
        if (currentState == GameState::Menu || currentState == GameState::Pause) {
            ChangeState(GameState::Quitting);
            Logger::Log("Game state is now Qutting");
        }
    }

    // Depuis Playing
    void TriggerPause() {
        if (currentState == GameState::Playing) {
            ChangeState(GameState::Pause);
            Logger::Log("Game state is now Pause");
        }
    }

    void TriggerGameOver() {
        if (currentState == GameState::Playing) {
            ChangeState(GameState::GameOver);
            Logger::Log("Game state is now Game Over");
        }
    }

    // Depuis Pause
    void TriggerResume() {
        if (currentState == GameState::Pause) {
            ChangeState(GameState::Playing);
            Logger::Log("Game state is now playing");
        }
    }

    // Depuis Game Over
    void TriggerRestart() {
        if (currentState == GameState::GameOver) {
            ChangeState(GameState::Playing);
            Logger::Log("Game state is now playing");
        }
    }

    // Retours communs vers le Menu (Option, Pause, Game Over -> Menu)
    void TriggerBackToMenu() {
        if (currentState == GameState::Option || 
            currentState == GameState::Pause || 
            currentState == GameState::GameOver) {
            ChangeState(GameState::Menu);
            Logger::Log("Game state is now Menu");
        }
    }
};