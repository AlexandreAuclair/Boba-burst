#include "bb_gameState.h"

GameStateMachine::GameStateMachine() : currentState(GameState::Menu) {}

GameState GameStateMachine::GetCurrentState() const {
    return currentState; 
}

void GameStateMachine::TriggerPlay() {
    if (currentState == GameState::Menu) {
        ChangeState(GameState::Playing);
        Logger::Log("Game state is now playing");
    }
}

void GameStateMachine::TriggerOption() {
    if (currentState == GameState::Menu) {
        ChangeState(GameState::Option);
        Logger::Log("Game state is now Option");
    }
}

void GameStateMachine::TriggerQuit() {
    if (currentState == GameState::Menu || currentState == GameState::Pause) {
        ChangeState(GameState::Quitting);
        Logger::Log("Game state is now Qutting");
    }
}

void GameStateMachine::TriggerPause() {
    if (currentState == GameState::Playing) {
        ChangeState(GameState::Pause);
        Logger::Log("Game state is now Pause");
    }
}

void GameStateMachine::TriggerGameOver() {
    if (currentState == GameState::Playing) {
        ChangeState(GameState::GameOver);
        Logger::Log("Game state is now Game Over");
    }
}

void GameStateMachine::TriggerResume() {
    if (currentState == GameState::Pause) {
        ChangeState(GameState::Playing);
        Logger::Log("Game state is now playing");
    }
}

void GameStateMachine::TriggerRestart() {
    if (currentState == GameState::GameOver) {
        ChangeState(GameState::Playing);
        Logger::Log("Game state is now playing");
    }
}

void GameStateMachine::TriggerBackToMenu() {
    if (currentState == GameState::Option || 
        currentState == GameState::Pause || 
        currentState == GameState::GameOver) {
        ChangeState(GameState::Menu);
        Logger::Log("Game state is now Menu");
    }
}