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

    //==========================================================================
    //      Constante
    //==========================================================================
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void Destroy();
private:
    void Setup();
    void ProcessInput();
    void Update();
    void Render();

private:
    bool isRunning;
    bool isDebug;
    GLFWwindow* window;
    int millisecsPreviousFrame = 0;
    GameStateMachine stateMachine;
    //Profiler profiler; // pour plus tard
};

#endif