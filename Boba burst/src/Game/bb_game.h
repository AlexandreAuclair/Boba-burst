#ifndef BB_GAME_H
#define BB_GAME_H

//==========================================================================
//      include externe
//==========================================================================
#include <GLFW/glfw3.h>
//==========================================================================
//      include interne
//==========================================================================
#include "bb_camera.h"
#include "bb_gameState.h"
#include "../LevelLoader/bb_levelLoader.h"
#include <src/EventBus/bb_eventBus.h>
#include <unordered_map>
#include "../Actor/Character/bb_player.h"
#include "bb_gameManager.h"
#include <algorithm>

//==========================================================================
//      Constante
//==========================================================================
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
    Game();
    ~Game();

public:
    static Game &get()
    {
        static Game singleton;
        return singleton;
    }

    void Initialize();
    void Run();
    void Destroy();

private:
    void Setup();
    void ProcessInput();
    void Update();
    void Render();
    void HandlePauseEvent(KeyPressedEvent &event);

private:
    bool isRunning;
    bool isDebug;
    GLFWwindow *window;
    float millisecsPreviousFrame = 0;
    GameStateMachine stateMachine;
    LevelLoader levelLoader;
    std::unique_ptr<EventBus> gameEventBus;
    //  Profiler profiler; // pour plus tard
};

#endif