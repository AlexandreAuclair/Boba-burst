#ifndef BB_GAME_H
#define BB_GAME_H

//==========================================================================
//      include externe
//==========================================================================
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
//==========================================================================
//      include interne
//==========================================================================
#include "bb_camera.h"
#include "bb_gameState.h"
#include "../LevelLoader/bb_levelLoader.h"
#include "../VulkanContext/bb_vulkanContext.h"
#include "../VulkanRenderer/bb_vulkanRenderer.h"
#include "../Controller/Controller.h"

//==========================================================================
//      Constante
//==========================================================================
const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
    Game();
    ~Game();

public:
    static Game& get() {
        static Game singleton;
        return singleton;
    }

    /* 
    init l'application et retourne vrai si elle a marcher parfaitement, 
    retourne faux si il y a une erreur
    */

    bool Initialize();
    void Run();
    void Destroy();

private:
    void Setup();
    void ProcessInput();
    void Update(double dt);
    void Render(double dt);

public : 

    Controller& GetController(){
        return controller;
    }

private:
    bool isRunning;
    bool isDebug;
    GLFWwindow* window;
    int millisecsPreviousFrame = 0;
    GameStateMachine stateMachine;
    LevelLoader levelLoader;
    VulkanContext context;
    std::unique_ptr<VulkanRenderer> renderer;
    std::shared_ptr<Scene> scene;
    //Profiler profiler; // pour plus tard

    // Controller d'inputs (init dans Setup())
    Controller controller;


    // Booléen indiquant si touche vient d'être pressee (pour eviter multi-action) => pourra etre change plus tard par fonction du controller
    bool tabWasPressed;
};

#endif